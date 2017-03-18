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

#include <GLFW/glfw3.h>

class OpenGLHelper{
public:
    OpenGLHelper();
    glm::vec3 getMousePosition(GLFWwindow* window);
};



#endif /* OpenGLHelper_hpp */
