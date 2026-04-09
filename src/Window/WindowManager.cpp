//
// Created by user on 09.04.2026.
//

#include "WindowManager.h"

#include <stdexcept>

namespace vulcraft {
    WindowManager::WindowManager() {
        if (!glfwInit()) {
            throw std::runtime_error("GLFW: failed to initialize!");
        }
    }

    void WindowManager::CreateWindow(uint32_t width, uint32_t height, const char* title) {
        glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
        glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );

        m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (!m_window) {
            throw std::runtime_error("GLFW: failed to create window!");
        }

        glfwMakeContextCurrent(m_window);
    }
} // vulcraft