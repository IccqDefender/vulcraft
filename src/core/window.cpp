#include "window.h"

#include <stdexcept>
#include "../utils/Logger.h"

void window::CreateWindow(uint32_t width, uint32_t height, const char* title) {
    if (!glfwInit()) {
        LOG_ERROR(Logger::Tag::Core, "Failed to initialize glfw!");
        __builtin_trap();
    }
    LOG_INFO(Logger::Tag::Core, "Initialize glfw..");

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE , GLFW_FALSE);

    m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!m_window) {
        LOG_ERROR(Logger::Tag::Core, "Failed to create window!");
        __builtin_trap();
    }
    LOG_INFO(Logger::Tag::Core, "Creating window..");
    glfwMakeContextCurrent(m_window);
}

void window::DestroyWindow() {
    glfwDestroyWindow(m_window);
}

void window::CleanupWindow() {
    DestroyWindow();
    glfwTerminate();
}
