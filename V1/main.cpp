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

#include "Points.hpp"
#include "OpenGLHelper.hpp"
#include "KDTree.hpp"

const bool useMouse = true;
const bool printFPS = true;
const bool printLoopTime = false;

int main( void )
{
    OpenGLHelper helper(printFPS, printLoopTime);
    
    if (!helper.initGLFWWindow(window)) {
        return 1;
    }
    

    // Create and compile our GLSL program from the shaders
    GLuint programID = helper.loadShaders(
        "TransformVertexShader.vertexshader",
        "ColorFragmentShader.fragmentshader"
    );
    
    int numberOfPoints = 20;
    KDTree tree(numberOfPoints, KDTree::BUILD_RECURSIVE_FAST, &helper, useMouse);
    
    double lastTimeTree = glfwGetTime();
    helper.initVertexBuffer();
    
    do{
        helper.registerLoop();
        
        lastTimeTree = glfwGetTime();
        
        tree.setData();
        int n = tree.getNumNodes();
        std::vector<float> v_data = tree.getData();
        
        double currentTimeTree = glfwGetTime();
        //printf("%f time for tree \n", currentTimeTree - lastTimeTree);

        
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Use our shader
        glUseProgram(programID);
        
        helper.passTextureToShader(n, v_data);
        helper.passNumberOfPointsToShader(numberOfPoints);
        helper.passKDTreeSizeToShader(n);
        
        helper.drawInLoop();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
          glfwWindowShouldClose(window) == 0 );
    
    return 0;
}
