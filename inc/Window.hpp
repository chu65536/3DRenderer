#pragma once

#include <vector>
#include <string>
#include <queue>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Object.hpp"
#include "Event.hpp"

class Window
{
public:
    Window(int width, int height, const std::string& title);
    bool IsOpen() const;
    float Update();
    void Close() const;
    void Settings(std::vector<Object>& objects, bool& lighting, bool& mode);
    GLFWwindow* GetWindow() const;
    bool PollEvents(Event& event);
private:
    void imguiInit() const;
    void cursorPosCallback(GLFWwindow* window, double xpos, double ypos); 
    void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    GLFWwindow* m_window;
    const int m_width;
    const int m_height;
    std::string m_title;

    std::queue<Event> m_events;

    float m_prevTime;
};