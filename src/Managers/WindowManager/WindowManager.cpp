#include "WindowManager.h"

void WindowManager::InitializeWindow() {
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE.c_str(), nullptr, nullptr);
}
