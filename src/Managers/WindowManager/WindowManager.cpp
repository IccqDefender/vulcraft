#include "WindowManager.h"

void WindowManager::InitializeWindow() {
    window = glfwCreateWindow(m_WINDOW_WIDTH, m_WINDOW_HEIGHT, m_WINDOW_TITLE.c_str(), nullptr, nullptr);
}