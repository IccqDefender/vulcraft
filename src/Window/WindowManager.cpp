#include "WindowManager.h"

WindowManager::WindowManager() {
    InitWindowManager();
}

void WindowManager::InitWindowManager() {
    glfwInit();

}

void WindowManager::CreateWindow() {
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    m_window = glfwCreateWindow(m_WINDOW_WIDTH, m_WINDOW_HEIGHT, "Vulcraft", nullptr, nullptr);
}

void WindowManager::Cleanup() {
    glfwTerminate();
}
