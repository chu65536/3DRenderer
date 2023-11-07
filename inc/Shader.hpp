#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader
{
public:
    Shader() = default;
    void Add(unsigned int type, const std::string& path);
    void Build();
    void Use() const;
    unsigned int GetID() const;
    void SetVec3(const std::string& label, const glm::vec3& vector);
    void SetVec4(const std::string& label, const glm::vec4& vector);
    void SetMat4(const std::string& label, const glm::mat4& matrix);
private:
    unsigned int m_programID;
    std::vector<unsigned int> m_shaders;

    void compile(unsigned int type, const std::string& source);
    bool checkSuccess(unsigned int shader, unsigned int type);
};