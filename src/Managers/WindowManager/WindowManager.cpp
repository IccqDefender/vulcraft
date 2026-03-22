#include "WindowManager.h"

void WindowManager::InitializeWindow(int width, int height, std::string title) {
    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
}
