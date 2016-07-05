#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <GL/glew.h>

class ShaderManager
{
public:
    static ShaderManager *getInstance();
    GLuint loadShader(GLenum shaderType, const std::string& shaderFile);
    GLuint loadShaderProgram(std::vector<GLuint> shaders);
        
private:
    ShaderManager();
    ~ShaderManager();

    static ShaderManager *instance;
};

