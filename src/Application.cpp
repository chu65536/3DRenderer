#include "Application.hpp"

#include <functional>

Application::Application(const std::vector<std::string>& objectsPaths):
    m_window(800, 800, "OpenGL")
{
    loadObjects(objectsPaths);
}

void Application::Run()
{   
    using namespace std::placeholders;
    auto settings = std::bind(&Window::Settings, m_window, _1, _2, _3);
    
    while(m_window.IsOpen())
    {
        pollEvents();
        float deltaTime = m_window.Update();
        m_renderer.Update(deltaTime);
        m_renderer.Render(m_window.GetWindow(), m_objects, settings);
    }
    m_window.Close();
}

void Application::loadObjects(const std::vector<std::string>& objectsPaths)
{   
    for (const auto& path: objectsPaths)
    {
        m_objects.emplace_back(path);
    }
}

void Application::pollEvents()
{
    Event event;
    while(m_window.PollEvents(event))
    {
        switch(event.type)
        {
        case Event::MouseMoved:
        {
            float xAngle = event.mouseMove.y / 3.f, yAngle = event.mouseMove.x / 3.f;
            m_renderer.RotateCamera(xAngle, yAngle, 0.f);
            break;
        }
        case Event::MouseScrolled:
        {
            float zOffset = event.mouseScroll.y / 10.f;
            m_renderer.MoveCamera(0.f, 0.f, zOffset);
            break;
        }
        }
    }    
}
