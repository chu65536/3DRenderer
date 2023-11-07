#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Camera
{
public:
    Camera();
    void Update(float xpos, float ypos);
    void SetProjection(float fov, float aspect, float near, float far);
    void SetPosition(float x, float y, float z);
    void SetRotation(float x, float y, float z);
    void Rotate(float x, float y, float z);
    void Move(float x, float y, float z);
    glm::mat4 GetProjection() const;
    glm::mat4 GetView() const;
private:
    glm::mat4 m_projection;
    glm::mat4 m_view;

    glm::vec3 m_position;
    glm::vec3 m_rotation;

    void updateView();
};