// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include "shader.hpp"

const int numberOfPoints = 100;

int init()
{
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }
    
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    
    
    // Open a window and create its OpenGL context
    window = glfwCreateWindow( 1024, 768, "Tutorial 04 - Colored Cube", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    
    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    
    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    
    return 0;
}

//return value from 0 to 1
float getRandom()
{
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

//return value from -1 to +1
float getRandomPosition()
{
    float r = getRandom();
    r = r*2 - 1;
    return r;
}

//return value from -0.001 to +0.001
float getRandomMove()
{
    float r = getRandom();
    r = r / 500;
    r = r *2 - 0.002;
    return r;
}

int main( void )
{
    // for random points
    time_t t;
    srand((unsigned) time(&t));
    
    int code = init();
    if (code) {
        return code;
    }
    
    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders( "TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader" );
    
    // Get a handle for our "MVP" uniform
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");
    
    // Get a handle for our buffers
    GLuint vsiPosition = glGetAttribLocation(programID, "vsiPosition");
    GLuint pointPositions = glGetUniformLocation(programID, "pointPositions");
    GLuint pointColors = glGetUniformLocation(programID, "pointColors");
    
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
    
    
    glm::vec3 pointNodePositions[numberOfPoints];
    glm::vec3 pointNodeMoves[numberOfPoints];
    glm::vec3 pointNodeColors[numberOfPoints];


    for (int i = 0; i < numberOfPoints; i++ )
    {
        pointNodePositions[i].x = getRandomPosition();
        pointNodePositions[i].y = getRandomPosition();
        pointNodePositions[i].z = 0;
        
        pointNodeMoves[i].x = getRandomMove();
        pointNodeMoves[i].y = getRandomMove();
        pointNodeMoves[i].z = 0;
        
        pointNodeColors[i].x = getRandom();
        pointNodeColors[i].y = getRandom();
        pointNodeColors[i].z = getRandom();
    }

    
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    
    int counter = 0;
    do{
        
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Use our shader
        glUseProgram(programID);
        
        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        
        // apply random move
        for (int i = 0; i < numberOfPoints; i++ )
        {
            pointNodePositions[i] += pointNodeMoves[i];
        }
        
        // check if we in area
        for (int i = 0; i < numberOfPoints; i++ )
        {
            pointNodePositions[i].x = pointNodePositions[i].x > 1
                ? pointNodePositions[i].x - 2
                : pointNodePositions[i].x;
            pointNodePositions[i].y = pointNodePositions[i].y > 1
                ? pointNodePositions[i].y - 2
                : pointNodePositions[i].y;
            pointNodePositions[i].x = pointNodePositions[i].x < -1
                ? pointNodePositions[i].x + 2
                : pointNodePositions[i].x;
            pointNodePositions[i].y = pointNodePositions[i].y < -1
                ? pointNodePositions[i].y + 2
                : pointNodePositions[i].y;
        }
        
        counter = counter == 1000 ? 0 : ++counter;
        for (int i = 0; i < numberOfPoints && counter == 0; i++ )
        {
            pointNodeMoves[i].x = getRandomMove();
            pointNodeMoves[i].y = getRandomMove();
        }
        
        glUniform3fv(pointPositions, numberOfPoints, glm::value_ptr(pointNodePositions[0]));
        glUniform3fv(pointColors, numberOfPoints, glm::value_ptr(pointNodeColors[0]));
        
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
