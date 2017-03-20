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
#include "Points.hpp"
#include "OpenGLHelper.hpp"

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

int main( void )
{    
    int code = init();
    if (code) {
        return code;
    }
    
    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders( "TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader" );
    
    
    // Get a handle for our buffers
    GLuint vsiPosition = glGetAttribLocation(programID, "vsiPosition");
    GLuint number = glGetUniformLocation(programID, "number");
    
    // Our vertices of QUAD
    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
    };
    
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    
    GLuint vertexTexture;
    GLuint   locTex = glGetUniformLocation(programID, "vertexTexture");
    int n = 127;
    float *data=new float[n * 4];
    
    //initialize data array with random integers between 1 and 100
    
    for(int i=0;i<n;i++)
    {
        data[i*4]=128;
        data[i*4+1]=0;
        data[i*4+2]=0;
        data[i*4+3]=128;
    }
    int i = 124;
    data[i*4]=255;
    data[i*4+1]=255;
    data[i*4+2]=255;
    data[i*4+3]=128;
    
    for(int i=0;i<n*4;i++)
    {
        data[i] /= 255.;
    }

//    for(int i=0;i<n;i++)
//    {
//        data[n+i]=0;
//        //        data[i*4+1]=0;
//        //        data[i*4+2]=128;
//        //        data[i*4+3]=255;
//    }
    
    //data[126] = 0;
    //data[125] = 0;
    
    
    //creat a texture and store data
    glGenTextures(1, &vertexTexture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, vertexTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, n, 1, 0, GL_RGBA, GL_FLOAT, data);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    int counter = 0;
    do{
        
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Use our shader
        glUseProgram(programID);
        
        glUniform1i(number, 5);
        glUniform1i(locTex, 0);
        
        
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
    
    delete[] data;
    return 0;
}
