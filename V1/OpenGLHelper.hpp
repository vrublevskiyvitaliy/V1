//
//  OpenGLHelper.hpp
//  V1
//
//  Created by Vitaliy Vrublevskiy on 3/18/17.
//  Copyright © 2017 Vitaliy Vrublevskiy. All rights reserved.
//

#ifndef OpenGLHelper_hpp
#define OpenGLHelper_hpp

#include <stdio.h>

#include <string>
#include <fstream>
#include <vector>

#include <glm/glm.hpp>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>


class OpenGLHelper{
    bool printFPS;
    bool printLoopTime;
    
    double lastFPSTime;
    double lastLoopTime;

    int numberOfFrames;
public:
    OpenGLHelper(bool _printFPS, bool _printLoopTime);
    static glm::vec3 getMousePosition(GLFWwindow* window);
    static bool initGLFWWindow(GLFWwindow * & window);
    static GLuint loadShaders(const char * vertex_file_path,const char * fragment_file_path);
    
    void startFPSCounter();
    void FPSCounter();
    
    void startLoopCounter();
    void loopCounter();

};

#endif /* OpenGLHelper_hpp */
