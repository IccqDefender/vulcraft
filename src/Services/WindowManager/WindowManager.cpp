#include "WindowManager.h"

WindowManager::WindowManager() {
    InitWindowManager();
}

WindowManager::~WindowManager() {
    glfwTerminate();
}

void WindowManager::InitWindowManager() {
    glfwInit();

}

void WindowManager::CreateWindow() {
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    g_window = glfwCreateWindow(m_WINDOW_WIDTH, m_WINDOW_HEIGHT, "Vulcraft", nullptr, nullptr);
}
