#include "VKEngine.h"

#include "GLFW/glfw3.h"

vulcraft::VKEngine::VKEngine() {
    _windowManager = new WindowManager();
    _instance = new Instance();
}

void vulcraft::VKEngine::Run() {
    _windowManager->CreateWindow(800, 600, "vulcraft"); // Create window

    _instance->InitVulkan(); // Init Vulkan

    MainLoop(_windowManager->GetWindow()); // Start main game loop
}

void vulcraft::VKEngine::MainLoop(GLFWwindow* window) {
    while (!_windowManager->IsWindowShouldClose(window)) {
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}

void vulcraft::VKEngine::Cleanup() {
    _instance->CleanupVulkan(); // Cleanup Vulkan

    glfwDestroyWindow(_windowManager->GetWindow()); // Cleanup window
    glfwTerminate();

    delete _windowManager;
    delete _instance;
}

void vulcraft::VKEngine::Stop() {
    Cleanup();
}
