cmake_minimum_required(VERSION 4.1)
project(vulcraft)

set(CMAKE_CXX_STANDARD 17)

find_package(Vulkan REQUIRED)

add_subdirectory(external/glfw)

add_executable(vulcraft
        src/main.cpp
        src/Window/WindowManager.cpp
        src/Window/WindowManager.h
        src/Renderer/Renderer.h
        src/Renderer/Pipeline.h
        src/Core/Device.h
        src/Renderer/Renderer.cpp
        src/Core/Device.cpp
        src/Renderer/Shader.cpp
        src/Renderer/Shader.h
        src/Renderer/Pipeline.cpp
        src/Core/Swapchain.cpp
        src/Core/Swapchain.h
        src/Core/VKengine.cpp
        src/Core/VKengine.h
)
target_sources(vulcraft PRIVATE
        external/imgui/imgui.cpp
        external/imgui/imgui_demo.cpp
        external/imgui/imgui_draw.cpp
        external/imgui/imgui_tables.cpp
        external/imgui/imgui_widgets.cpp
        # Бэкенды для GLFW и Vulkan
        external/imgui/backends/imgui_impl_glfw.cpp
        external/imgui/backends/imgui_impl_vulkan.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan glfw)

target_include_directories(${PROJECT_NAME} PRIVATE external/glfw/include external/imgui external/imgui/backends)