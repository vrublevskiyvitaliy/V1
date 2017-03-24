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

const bool useMouse = false;
const bool printFPS = true;
const bool printLoopTime = true;

int main( void )
{
    
    if (!OpenGLHelper::initGLFWWindow(window)) {
        return 1;
    }
    
    OpenGLHelper helper(printFPS, printLoopTime);

    // Create and compile our GLSL program from the shaders
    GLuint programID = helper.loadShaders(
        "TransformVertexShader.vertexshader",
        "ColorFragmentShader.fragmentshader"
    );
    
    KDTree tree;
    
    double lastTimeTree = glfwGetTime();
    helper.initVertexBuffer();

    do{
        helper.registerLoop();
        
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
        
        //printf("%f time for tree \n", currentTimeTree - lastTimeTree);
        
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Use our shader
        glUseProgram(programID);
        
        helper.passTextureToShader(n, data);
        
        glUniform2f(glGetUniformLocation(programID, "UN_SAMP_KDTREE_SIZE"), double(n), 1.);
        
        helper.drawInLoop(window);

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
          glfwWindowShouldClose(window) == 0 );
    
    return 0;
}
