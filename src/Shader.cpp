#include "Shader.hpp"
#include <glm/gtc/type_ptr.hpp>


void Shader::Add(unsigned int type, const std::string& path)
{
    std::ifstream sourceFile(path);
    if (!sourceFile)
    {
        std::cout << "Error: failed to open shader file" << std::endl;
        return;
    }

    std::stringstream buffer;
    buffer << sourceFile.rdbuf();
    std::string sourceCode = buffer.str();
    compile(type, sourceCode);
}

void Shader::Build()
{
    m_programID = glCreateProgram();
    for (auto shader: m_shaders)
    {
        glAttachShader(m_programID, shader);
    }

    glLinkProgram(m_programID);
    int success;
    char infoLog[512];
    if (!success)
    {
        glGetProgramInfoLog(m_programID, 512, NULL, infoLog);
        std::cout << "Failed to link shader program\n" << infoLog << std::endl;
    }

    for (auto shader: m_shaders)
    {
        glDeleteShader(shader);
    }
}

void Shader::Use() const
{
    glUseProgram(m_programID);
}

void Shader::compile(unsigned int type, const std::string& source)
{   
    const char* shaderSource = source.c_str();
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);
    bool ok = checkSuccess(shader, type);
    if (ok)
    {
        m_shaders.emplace_back(shader);
    }
}

bool Shader::checkSuccess(unsigned int shader, unsigned int type) 
{
    std::string sType;
    switch(type) 
    {
    case GL_VERTEX_SHADER:
        sType = "VERTEX";
        break;
    case GL_FRAGMENT_SHADER:
        sType = "FRAGMENT";
        break;
    }

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) 
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR: " << sType << " shader compilation failed.\n" << infoLog << std::endl;
        return false;
    }

    std::cout << sType << " shader compiled successfully!" << std::endl;
    return true;
}

unsigned int Shader::GetID() const
{
    return m_programID;
}

void Shader::SetMat4(const std::string& label, const glm::mat4& matrix)
{
    unsigned int location = glGetUniformLocation(m_programID, label.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::SetVec3(const std::string& label, const glm::vec3& vector)
{
    unsigned int location = glGetUniformLocation(m_programID, label.c_str());
    glUniform3fv(location, 1, glm::value_ptr(vector));
}

void Shader::SetVec4(const std::string& label, const glm::vec4& vector)
{
    unsigned int location = glGetUniformLocation(m_programID, label.c_str());
    glUniform4fv(location, 1, glm::value_ptr(vector));
}