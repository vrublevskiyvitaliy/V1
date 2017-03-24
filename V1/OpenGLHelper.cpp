//
//  OpenGLHelper.cpp
//  V1
//
//  Created by Vitaliy Vrublevskiy on 3/18/17.
//  Copyright © 2017 Vitaliy Vrublevskiy. All rights reserved.
//

#include "OpenGLHelper.hpp"

glm::vec3 OpenGLHelper::getMousePosition(GLFWwindow* window)
{
    
    double xpos, ypos;
    
    glfwGetCursorPos(window, &xpos, &ypos);
    
    int xSize = 1024, ySize = 768;
    
    glfwGetWindowSize(window, &xSize, &ySize);
    
    glm::vec3 mouse;
    
    mouse.x = ( xpos * 2.0 ) / ( xSize ) - 1;
    mouse.y = ( ypos * 2.0 ) / ( ySize ) - 1;
    
    // invert y
    // because glfwGetCursorPos return position from upper-left corner
    
    mouse.y = - mouse.y;
    
    mouse.z = 0;
    
    return mouse;
}

bool OpenGLHelper::initGLFWWindow(GLFWwindow * & window)
{
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return false;
    }
    
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    
    // Open a window and create its OpenGL context
    window = glfwCreateWindow( 1024, 768, "Shader Voronoi diagram", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return false;
    }
    
    glfwMakeContextCurrent(window);
    
    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return false;
    }
    
    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    
    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    
    return true;
}
