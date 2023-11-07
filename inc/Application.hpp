#pragma once

#include "Window.hpp"
#include "Renderer.hpp"
#include "Object.hpp"

class Application
{
public:
    Application(const std::vector<std::string>& objectsPaths);
    void Run();
private:
    void loadObjects(const std::vector<std::string>& objectsPaths);
    void pollEvents();

    Window m_window;
    Renderer m_renderer;
    std::vector<Object> m_objects;
};