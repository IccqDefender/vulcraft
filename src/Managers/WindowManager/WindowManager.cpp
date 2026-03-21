//
// Created by user on 21.03.2026.
//

#include "WindowManager.h"

void WindowManager::CreateWindow() {
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE.c_str(), nullptr, nullptr);
}
