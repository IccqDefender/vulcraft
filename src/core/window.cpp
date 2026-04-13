#include "window.h"

#include <stdexcept>

void window::CreateWindow(uint32_t width, uint32_t height, const char* title) {
    if (!glfwInit()) {
        throw std::runtime_error("[GLFW]: failed to initialize GLFW!");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE , GLFW_FALSE);

    m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!m_window) {
        throw std::runtime_error("[GLFW]: failed to create window!");
    }
    glfwMakeContextCurrent(m_window);
}

void window::DestroyWindow() {
    glfwDestroyWindow(m_window);
}

void window::CleanupWindow() {
    DestroyWindow();
    glfwTerminate();
}
