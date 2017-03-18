//
//  OpenGLHelper.cpp
//  V1
//
//  Created by Vitaliy Vrublevskiy on 3/18/17.
//  Copyright © 2017 Vitaliy Vrublevskiy. All rights reserved.
//

#include "OpenGLHelper.hpp"

OpenGLHelper::OpenGLHelper() {
    
}

glm::vec3 OpenGLHelper::getMousePosition(GLFWwindow* window) {
    double xpos, ypos;
    
    glfwGetCursorPos(window, &xpos, &ypos);
    
    int xSize = 1024, ySize = 768;
    
    glfwGetWindowSize(window, &xSize, &ySize);
    
    glm::vec3 mouse;
    
    mouse.x = ( xpos * 2.0 ) / ( xSize ) - 1;
    mouse.y = ( ypos * 2.0 ) / ( ySize) - 1;
    
    // invert y
    // because glfwGetCursorPos return position from upper-left corner
    mouse.y = - mouse.y;
    
    mouse.z = 0;
    
    return mouse;
}
