#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Renderer.hpp"


Renderer::Renderer()
{   
    glEnable(GL_DEPTH_TEST);

    loadShaders();
    setupCamera();

    m_shaders[0].Use();
    m_shaders[0].SetVec3("lightColor",  glm::vec3(1.0f, 1.0f, 1.0f));
    m_shaders[0].SetVec3("lightPos", glm::vec3(0.f, 0.f, -10.f)); 

    m_lighting = true;
    m_frameMode = false;
}

void Renderer::Update(float dt)
{   
    unsigned int lightingLoc = glGetUniformLocation(m_shaders[0].GetID(), "isLighting");
    glUniform1i(lightingLoc, m_lighting);
    if (m_frameMode)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void Renderer::Render(GLFWwindow* window, std::vector<Object>& objects, SettingsFunctor settings) 
{   
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_shaders[0].SetMat4("view", m_camera.GetView());
    m_shaders[0].SetMat4("projection", m_camera.GetProjection());
    for (auto& object: objects)
    {
        m_shaders[0].SetMat4("model", object.GetModel());
        m_shaders[0].SetVec3("objectColor", object.GetColor());
        object.Draw();
    }

    settings(objects, m_lighting, m_frameMode);

    glfwSwapBuffers(window);
}

void Renderer::RotateCamera(float x, float y, float z)
{
    m_camera.Rotate(x, y, z);
}

void Renderer::MoveCamera(float x, float y, float z)
{
    m_camera.Move(x, y, z);
}

void Renderer::loadShaders()
{   
    Shader defaultShader;
    defaultShader.Add(GL_VERTEX_SHADER, "../res/shaders/vertex.glsl");
    defaultShader.Add(GL_FRAGMENT_SHADER, "../res/shaders/fragment.glsl");
    defaultShader.Build();
    m_shaders.emplace_back(defaultShader);
} 

void Renderer::setupCamera()
{   
    m_camera.SetProjection(45.f, 1.f, 0.1f, 10.f);
    m_camera.SetPosition(0.f, 0.f, -2.f);
    m_camera.SetRotation(0.f, 0.f, 0.f);
}