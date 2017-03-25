//
//  OpenGLHelper.cpp
//  V1
//
//  Created by Vitaliy Vrublevskiy on 3/18/17.
//  Copyright © 2017 Vitaliy Vrublevskiy. All rights reserved.
//

#include "OpenGLHelper.hpp"


OpenGLHelper::OpenGLHelper(bool _printFPS, bool _printLoopTime)
{
    printFPS = _printFPS;
    printLoopTime = _printLoopTime;
    
    lastLoopTime = glfwGetTime();
    lastFPSTime = glfwGetTime();
    numberOfFrames = 0;
}

void OpenGLHelper::registerLoop()
{

    if (printLoopTime) loopTime();
    if (printFPS) FPSCounter();
    
    
}


void OpenGLHelper::FPSCounter()
{
    numberOfFrames++;
    double currentFPSTime = glfwGetTime();
    if ( currentFPSTime - lastFPSTime >= 1.0 ){ // If last prinf() was more than 1 sec ago
        // printf and reset timer
        printf("%f ms/frame or %d fps\n", 1000.0/double(numberOfFrames), numberOfFrames);
        numberOfFrames = 0;
        lastFPSTime += 1.;
    }
}

void OpenGLHelper::loopTime()
{
    double currentLoopTime = glfwGetTime();
    printf("%f time for loop\n", currentLoopTime - lastLoopTime);
    
    lastLoopTime = glfwGetTime();
}

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

void OpenGLHelper::passTextureToShader(int n, std::vector<float> data)
{
    GLuint vertexTexture;
    
    glGenTextures(1, &vertexTexture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, vertexTexture);
    // todo: maybe find better solution than &data[0]
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, n, 1, 0, GL_RGBA, GL_FLOAT, &data[0]);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glUniform1i(glGetUniformLocation(programID, "texture"), 0);
    
}

void OpenGLHelper::initVertexBuffer() {
    
    // Our vertices of QUAD
    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
    };
    
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    
}

void OpenGLHelper::drawInLoop(GLFWwindow* window)
{
    GLuint position = glGetAttribLocation(programID, "position");
    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(position);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
                          position, // The attribute we want to configure
                          3,                           // size
                          GL_FLOAT,                    // type
                          GL_FALSE,                    // normalized?
                          0,                           // stride
                          (void*)0                     // array buffer offset
                          );
    
    // Draw the QUADS
    glDrawArrays(GL_QUADS, 0, 4);
    
    glDisableVertexAttribArray(position);
    
    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
    
}

OpenGLHelper::~OpenGLHelper()
{
    // Cleanup VBO and shader
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteProgram(programID);
    
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
}

void OpenGLHelper::passKDTreeSizeToShader(int n) {
    glUniform2f(glGetUniformLocation(programID, "KDTREE_SIZE"), double(n), 1.);
}

void OpenGLHelper::passNumberOfPointsToShader(int n) {
    glUniform1f(glGetUniformLocation(programID, "numberOfPoints"), double(n));
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

GLuint OpenGLHelper::loadShaders(const char * vertex_file_path,const char * fragment_file_path) {
    
    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    
    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    if(VertexShaderStream.is_open()){
        std::string Line = "";
        while(getline(VertexShaderStream, Line))
            VertexShaderCode += "\n" + Line;
        VertexShaderStream.close();
    }else{
        printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
        getchar();
        return 0;
    }
    
    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if(FragmentShaderStream.is_open()){
        std::string Line = "";
        while(getline(FragmentShaderStream, Line))
            FragmentShaderCode += "\n" + Line;
        FragmentShaderStream.close();
    }
    
    GLint Result = GL_FALSE;
    int InfoLogLength;
    
    
    // Compile Vertex Shader
    printf("Compiling shader : %s\n", vertex_file_path);
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);
    
    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }
    
    
    
    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fragment_file_path);
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);
    
    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
    }
    
    
    
    // Link the program
    printf("Linking program\n");
    programID = glCreateProgram();
    glAttachShader(programID, VertexShaderID);
    glAttachShader(programID, FragmentShaderID);
    glLinkProgram(programID);
    
    // Check the program
    glGetProgramiv(programID, GL_LINK_STATUS, &Result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> ProgramErrorMessage(InfoLogLength+1);
        glGetProgramInfoLog(programID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }
    
    
    glDetachShader(programID, VertexShaderID);
    glDetachShader(programID, FragmentShaderID);
    
    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);
    
    return programID;
}
