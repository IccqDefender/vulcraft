#include "vk.h"

void vk::Run() {
    _window = new window();
    _window->CreateWindow(WIDTH, HEIGHT, TITLE);
    while (!_window->ShouldClose()) {
        MainLoop();
    }
}

void vk::MainLoop() {
    glfwPollEvents();
}

void vk::Stop() {
    _window->CleanupWindow();
    delete _window;
}