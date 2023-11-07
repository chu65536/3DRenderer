#pragma once

#include <vector>
#include <string>
#include <array>

#include "glm/glm.hpp"


struct Mesh
{
    std::vector<float> positions;
    std::vector<float> normals;
    std::vector<unsigned int> indices;
};

class Object
{
public:
    Object(const std::string& path);
    void Draw();
    void SetPosition(float x, float y, float z);
    void SetRotation(float x, float y, float z);
    void SetScale(float x, float y, float z);
    void SetColor(float r, float g, float b);
    glm::mat4 GetModel() const;
    glm::vec3 GetColor() const;
    std::array<float, 3> GetPosition();
    std::array<float, 3> GetRotation();  
    std::array<float, 3> GetScale();

    void Debug();
private:
    void loadModel(const std::string& path);
    void loadTexture(const std::string& path);
    void initBuffers();
    void scaleNormalize(float lX, float rX, float lY, float rY, float lZ, float rZ);
    void updateModel();

    std::vector<Mesh> m_meshes;

    glm::mat4 m_model;
    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;
    glm::vec3 m_color;

    std::vector<unsigned int> m_VAO;
};