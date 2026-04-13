#include "vk.h"

#include <memory>

#include "../utils/Logger.h"

void vk::Run() {
    LOG_INFO(Logger::Tag::Core, "Running vulcraft..");

    m_running = true;

    _window = std::make_unique<window>();
    _window->CreateWindow(WIDTH, HEIGHT, TITLE);

    LOG_INFO(Logger::Tag::Core, "Start main loop..");
    while (m_running && !_window->ShouldClose()) {
        MainLoop();
    }
}

void vk::MainLoop() {
    if (glfwGetKey(_window->GetCurrentWindow(),GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        Stop();
    }

    glfwPollEvents();
}

void vk::Stop() {
    LOG_INFO(Logger::Tag::Core, "Stoping vulcraft..");

    m_running = false;

    if (_window) {
        _window->CleanupWindow();
        _window.reset();
    }
}