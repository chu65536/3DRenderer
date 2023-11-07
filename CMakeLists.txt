cmake_minimum_required(VERSION 3.22.1)

project(computer-graphics)

include(FetchContent)
set(BUILD_SHARED_LIBS OFF)

# glfw
FetchContent_Declare(
    glfw3
    GIT_REPOSITORY https://github.com/glfw/glfw.git
)
FetchContent_MakeAvailable(glfw3)

# imgui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
)
FetchContent_MakeAvailable(imgui)
set(
    IMGUI_SRC
    _deps/imgui-src/backends/imgui_impl_opengl3.h
    _deps/imgui-src/backends/imgui_impl_opengl3.cpp
    _deps/imgui-src/backends/imgui_impl_opengl3_loader.h
    _deps/imgui-src/backends/imgui_impl_glfw.h
    _deps/imgui-src/backends/imgui_impl_glfw.cpp
    _deps/imgui-src/imgui.h
    _deps/imgui-src/imgui.cpp
    _deps/imgui-src/imgui_draw.cpp
    _deps/imgui-src/imgui_tables.cpp
    _deps/imgui-src/imgui_widgets.cpp
    _deps/imgui-src/imgui_demo.cpp
)

# glad
add_library(glad ext/glad/glad.h ext/glad/glad.c)

# build
add_executable(
    computer-graphics
    src/main.cpp
    src/Shader.cpp
    src/Renderer.cpp
    src/Object.cpp
    src/Camera.cpp
    src/Application.cpp
    src/Window.cpp
    ${IMGUI_SRC}
)

target_include_directories(
    computer-graphics
    PRIVATE
    inc
    ext
    build/_deps/imgui-src
    build/_deps/imgui-src/backends
)

target_link_libraries(
    computer-graphics
    PRIVATE
    GL
    glad
    glfw
)
