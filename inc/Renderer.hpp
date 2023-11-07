#pragma once

#include <string>
#include <array>
#include <functional>

#include "Shader.hpp"
#include "Object.hpp"
#include "Camera.hpp"

#include <GLFW/glfw3.h>

using SettingsFunctor = std::function<void(std::vector<Object>&, bool& lighting, bool& mode)>;

class Renderer
{
public:
    Renderer();
    void Update(float deltaTime);
    void Render(GLFWwindow* window, std::vector<Object>& object, SettingsFunctor settings);
    void RotateCamera(float x, float y, float z);
    void MoveCamera(float x, float y, float z);
private:
    std::vector<Shader> m_shaders;
    Camera m_camera;
    bool m_lighting;
    bool m_frameMode;

    void loadShaders();
    void setupCamera();
};