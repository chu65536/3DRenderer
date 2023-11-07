#include "Camera.hpp"


float prevXpos = 0.f;
float prevYpos = 0.f;

Camera::Camera():
    m_projection(glm::mat4(1.0f)), m_view(glm::mat4(1.0f)), m_position(glm::vec3(0.0f)), m_rotation(glm::vec3(0.0f))
{

}

void Camera::Update(float xpos, float ypos)
{
    float difX = xpos - prevXpos;
    float difY = ypos - prevYpos;

    SetRotation(m_rotation.x, m_rotation.y + difX, m_rotation.z);

    prevXpos = xpos;
    prevYpos = ypos;
}

void Camera::SetProjection(float fov, float aspect, float near, float far)
{
    m_projection = glm::perspective(fov, aspect, near, far);
}

void Camera::SetPosition(float x, float y, float z)
{   
    m_position = glm::vec3(x, y, z);
    updateView();
}

void Camera::SetRotation(float x, float y, float z)
{
    m_rotation = glm::vec3(x, y, z);
    updateView();
}

void Camera::Rotate(float x, float y, float z)
{
    m_rotation += glm::vec3(x, y, z);
    updateView();
}

void Camera::Move(float x, float y, float z)
{
    m_position += glm::vec3(x, y, z);
    updateView();
}

void Camera::updateView()
{
    m_view = glm::mat4(1.0f);
    m_view = glm::translate(m_view, m_position);
    m_view = glm::rotate(m_view, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    m_view = glm::rotate(m_view, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    m_view = glm::rotate(m_view, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
}

glm::mat4 Camera::GetProjection() const
{
    return m_projection;
}

glm::mat4 Camera::GetView() const
{
    return m_view;
}