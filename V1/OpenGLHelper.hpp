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

#include <glm/glm.hpp>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

class OpenGLHelper{
public:
    static glm::vec3 getMousePosition(GLFWwindow* window);
    static bool initGLFWWindow(GLFWwindow * & window);
};

#endif /* OpenGLHelper_hpp */
