#include "ShaderManager.h"

ShaderManager* ShaderManager::instance = nullptr;

ShaderManager::ShaderManager()
{
}


ShaderManager::~ShaderManager()
{
}

ShaderManager* ShaderManager::getInstance()
{
    if (!instance)
    {
        instance = new ShaderManager();
    }

    return instance;
}

GLuint ShaderManager::loadShader(GLenum shaderType, const std::string& shaderFile)
{
    std::ifstream ifs;

    // Load the shader.
    ifs.open(shaderFile);

    if (!ifs)
    {
        std::cerr << "Can not open shader file: \"" << shaderFile << "\"" << std::endl;
        return 0;
    }

    std::string source(std::istreambuf_iterator<char>(ifs), (std::istreambuf_iterator<char>()));
    ifs.close();

    // Create a shader object.
    GLuint shader = glCreateShader(shaderType);

    // Load the shader source for each shader object.
    const GLchar* sources[] = { source.c_str() };
    glShaderSource(shader, 1, sources, NULL);

    // Compile the shader.
    glCompileShader(shader);

    // Check for errors
    GLint compileStatus;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus != GL_TRUE)
    {
        GLint logLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        GLchar* infoLog = new GLchar[logLength];
        glGetShaderInfoLog(shader, logLength, NULL, infoLog);

#ifdef _WIN32
        std::cout << "load shader: " << infoLog;
        //OutputDebugString(infoLog);
#else
        std::cerr << infoLog << std::endl;
#endif
        delete infoLog;
        return 0;
    }

    //std::cout << "load shader successed" << std::endl;

    return shader;
}

GLuint ShaderManager::loadShaderProgram(std::vector<GLuint> shaders)
{
    // Create a shader program.
    GLuint program = glCreateProgram();

    // Attach the appropriate shader objects.
    for (GLuint shader : shaders)
    {
        glAttachShader(program, shader);
    }

    // Link the program
    glLinkProgram(program);

    // Check the link status.
    GLint linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if (linkStatus != GL_TRUE)
    {
        GLint logLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        GLchar* infoLog = new GLchar[logLength];

        glGetProgramInfoLog(program, logLength, NULL, infoLog);

#ifdef _WIN32
        std::cout << "load shader program: " << infoLog;
#else
        std::cerr << infoLog << std::endl;
#endif

        delete infoLog;
        return 0;
    }

    //std::cout << "load shader program successed" << std::endl;

    return program;
}