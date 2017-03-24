// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

GLFWwindow * window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include "shader.hpp"
#include "Points.hpp"
#include "OpenGLHelper.hpp"
#include "KDTree.hpp"

const int numberOfPoints = 100;
const bool useMouse = false;

int main( void )
{
    if (!OpenGLHelper::initGLFWWindow(window)) {
        return 1;
    }
    
    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders( "TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader" );
    
    // Get a handle for our "MVP" uniform
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");
    
    // Get a handle for our buffers
    GLuint vsiPosition = glGetAttribLocation(programID, "vsiPosition");
    GLuint pointPositions = glGetUniformLocation(programID, "pointPositions");
    GLuint kdTreeSize = glGetUniformLocation(programID, "UN_SAMP_KDTREE_SIZE");
    
    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model      = glm::mat4(1.0f);
    // Our ModelViewProjection : multiplication of our 3 matrices
    glm::mat4 MVP        = Model; // Remember, matrix multiplication is the other way around
    
    // Our vertices of QUAD
    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
    };
    
    glm::vec3 * pointNodePositions;
    
    Points points(numberOfPoints);
    
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    
    GLuint vertexTexture;
    GLuint locTex = glGetUniformLocation(programID, "vertexTexture");
    
    KDTree tree;
    
    double lastTime = glfwGetTime(); int nbFrames = 0;
    double lastTimeTree = glfwGetTime();
    double lastTimeLoop = glfwGetTime();
    int counter = 0;
    do{
        double currentTime = glfwGetTime();
        double currentTimeLoop = glfwGetTime();
        printf("%f time for loop\n", currentTimeLoop - lastTimeLoop);
        lastTimeLoop = glfwGetTime();
        nbFrames++;
        if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1 sec ago
            // printf and reset timer
            printf("%f ms/frame or %f fps\n", 1000.0/double(nbFrames), double(nbFrames));
            nbFrames = 0;
            lastTime += 1.0;
        }
        
        
        lastTimeTree = glfwGetTime();
            tree.setData();
            int n = tree.getNumNodes();
            std::vector<float> v_data = tree.getData();
        
            float * data = new float[n * 4];
        
            for(int i=0; i < n * 4; i++)
            {
                data[i]=v_data[i];
            }
        double currentTimeTree = glfwGetTime();
        
        printf("%f time for tree \n", currentTimeTree - lastTimeTree);

            glm::vec2 kdTreeSizeValue = glm::vec2(n, 1);
        
            glGenTextures(1, &vertexTexture);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, vertexTexture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, n, 1, 0, GL_RGBA, GL_FLOAT, data);
        
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Use our shader
        glUseProgram(programID);
        
        glUniform1i(locTex, 0);
        glUniform2f(kdTreeSize, kdTreeSizeValue.x, kdTreeSizeValue.y);
        
        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        
        points.applyMove();
        
        pointNodePositions = points.getPointsPositions();
        // our mouse
        if (useMouse) {
            pointNodePositions[numberOfPoints - 1] = OpenGLHelper::getMousePosition(window);
        }
        
        glUniform3fv(pointPositions, numberOfPoints, glm::value_ptr(pointNodePositions[0]));
        
        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(vsiPosition);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                              vsiPosition, // The attribute we want to configure
                              3,                           // size
                              GL_FLOAT,                    // type
                              GL_FALSE,                    // normalized?
                              0,                           // stride
                              (void*)0                     // array buffer offset
        );
        
        // Draw the QUADS
        glDrawArrays(GL_QUADS, 0, 4);
        
        glDisableVertexAttribArray(vsiPosition);
        
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        
        
    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
          glfwWindowShouldClose(window) == 0 );
    
    // Cleanup VBO and shader
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteProgram(programID);
    
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    
    return 0;
}
