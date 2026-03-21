#include "Engine.h"

void Engine::Run() {
    InitGLFW();

    auto* _windowManager = new WindowManager();
    _windowManager->CreateWindow();

    glfwMakeContextCurrent( _windowManager->window );

    MainLoop(_windowManager);

    Cleanup(_windowManager);
}

void Engine::MainLoop( WindowManager *windowManager ) {
    while (!glfwWindowShouldClose( windowManager->window ) ) {
        glfwSwapBuffers( windowManager->window );
        glfwPollEvents();
    }
}

void Engine::InitGLFW() {
    glfwInit();
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }
}

void Engine::Cleanup( WindowManager *windowManager ) {
    glfwDestroyWindow( windowManager->window );
    glfwTerminate();
}
