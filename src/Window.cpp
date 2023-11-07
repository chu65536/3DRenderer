#include "Window.hpp"

#include <stdexcept>
#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


float prevXPos = 0.f;
float prevYPos = 0.f;

void Window::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) 
{   
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) 
    {   
        prevXPos = xpos;
        prevYPos = ypos;
        return;
    }

    float deltaX = xpos - prevXPos;
    float deltaY = ypos - prevYPos;
    prevXPos = xpos;
    prevYPos = ypos;

    Event event;
    event.type = Event::MouseMoved;
    event.mouseMove = Event::MouseMoveEvent{deltaX, deltaY};
    m_events.emplace(event);
}

void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    Event event;
    event.type = Event::MouseScrolled;
    event.mouseScroll = Event::MouseScrollEvent{(float)xoffset, (float)yoffset};
    m_events.emplace(event);  
}

Window::Window(int width, int height, const std::string& title):
    m_width(width), m_height(height), m_title(title), m_prevTime(0.f)
{   
    glfwInit();
    m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), NULL, NULL);
    if (m_window == NULL)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(m_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw std::runtime_error("Failed to initialize glad");
    }

    glfwSetWindowUserPointer(m_window, this);
    auto cursorPosFunc = [](GLFWwindow* w, double xpos, double ypos)
    {
        static_cast<Window*>(glfwGetWindowUserPointer(w))->cursorPosCallback(w, xpos, ypos);
    };
    glfwSetCursorPosCallback(m_window, cursorPosFunc);

    auto scrollFunc = [](GLFWwindow* w, double xoffset, double yoffset)
    {
        static_cast<Window*>(glfwGetWindowUserPointer(w))->scrollCallback(w, xoffset, yoffset);
    };
    glfwSetScrollCallback(m_window, scrollFunc);

    imguiInit();
}

bool Window::PollEvents(Event& event)
{
    if (m_events.empty())
    {
        return false;
    }

    event = m_events.front();
    m_events.pop();
    return true;
}

bool Window::IsOpen() const
{
    return !glfwWindowShouldClose(m_window);
}

void Window::Close() const
{
    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
}

float Window::Update()
{   
    float curTime = float(glfwGetTime());
    float deltaTime = curTime - m_prevTime;
    m_prevTime = curTime;

    glfwPollEvents();

    return deltaTime;
}

void Window::Settings(std::vector<Object>& objects, bool& lighting, bool& mode)
{   
    ImGui_ImplGlfw_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    int cnt = 1;
    ImGui::Begin("Settings");

    ImGui::Checkbox("Lighting", &lighting);
    ImGui::Checkbox("Frame mode", &mode);
    ImGui::Separator();

    for (auto& object: objects)
    {   
        std::string title = "Object " + std::to_string(cnt);
        ImGui::Text(title.c_str());

        std::string posLabel = "Position##" + std::to_string(cnt);
        std::array<float, 3> position = object.GetPosition();
        ImGui::DragFloat3(posLabel.c_str(), position.data(), 0.01f, -3.f, 3.f);
        object.SetPosition(position[0] ,position[1], position[2]);

        std::string rotLabel = "Rotation##" + std::to_string(cnt);
        std::array<float, 3> rotation = object.GetRotation();
        ImGui::DragFloat3(rotLabel.c_str(), rotation.data(), 0.5f, -180.f, 180.f);
        object.SetRotation(rotation[0] ,rotation[1], rotation[2]);

        std::string sclLabel = "Scale##" + std::to_string(cnt);
        std::array<float, 3> scale = object.GetScale();
        ImGui::DragFloat3(sclLabel.c_str(), scale.data(), 0.01f, 0.01f, 1.f);
        object.SetScale(scale[0] ,scale[1], scale[2]);

        std::string colLabel = "Color##" + std::to_string(cnt);
        glm::vec3 col = object.GetColor();
        std::array<float, 3> color = {col.x, col.y, col.z};
        ImGui::ColorPicker3(colLabel.c_str(), color.data());
        object.SetColor(color[0], color[1], color[2]);
        ImGui::Separator();

        cnt++;
    }
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Window::imguiInit() const
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 460");
}

GLFWwindow* Window::GetWindow() const
{
    return m_window;
}