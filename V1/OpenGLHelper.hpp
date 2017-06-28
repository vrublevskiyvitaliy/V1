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
    
    double lastLoopTime;
    double lastFPSTime;

    int numberOfFrames;
    
    GLuint programID;
    GLuint vertexbuffer;
    
    GLFWwindow * window;
public:
    OpenGLHelper(bool _printFPS, bool _printLoopTime);
    glm::vec3 getMousePosition();
    bool initGLFWWindow(GLFWwindow * & _window);
    GLuint loadShaders(const char * vertex_file_path,const char * fragment_file_path);

    void passTextureToShader(int n, std::vector<float> data, std::vector<float> color);
    
    void registerLoop();
    void FPSCounter();
    void loopTime();
    
    
    void initVertexBuffer();
    void drawInLoop();
    
    void passKDTreeSizeToShader(int n);
    void passNumberOfPointsToShader(int n);
    
    
    ~OpenGLHelper();
};

#endif /* OpenGLHelper_hpp */
