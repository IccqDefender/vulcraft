#include "Engine.h"

/* RUN ENGINE */
void Engine::Run() {

    InitGLFW(); // INIT GLFW

    glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    auto* _windowManager = new WindowManager();       // MAKE OBJ OF WINDOW_MANAGER
    _windowManager->InitializeWindow(_windowManager->WINDOW_WIDTH, _windowManager->WINDOW_HEIGHT, _windowManager->WINDOW_TITLE);

    glfwMakeContextCurrent( _windowManager->window ); // MAKE CONTEXT WINDOW

    auto* _vulkanManager = new VulkanManager();       // MAKE OBJ OF VULKAN_MANAGER
    _vulkanManager->InitVulkan(_windowManager);

    auto* _imGuiManager = new ImGuiManager();         // MAKE OBJ OF IM_GUI_MANAGER
    _imGuiManager->InitImGui(_windowManager);

    /* MAIN LOOP */
    MainLoop(_windowManager, _vulkanManager, _imGuiManager);

    /* CLEANUP */
    Cleanup(_windowManager, _vulkanManager, _imGuiManager);
}

/* MAIN LOOP */
void Engine::MainLoop( WindowManager *windowManager, VulkanManager *vulkanManager, ImGuiManager *imGuiManager ) {
    while (!glfwWindowShouldClose( windowManager->window ) ) {
        glfwPollEvents();
        glfwSwapBuffers( windowManager->window );
    }
}

/* INIT GLFW */
void Engine::InitGLFW() {
    glfwInit();
    if (!glfwInit()) {
        throw std::runtime_error("[GLFW]: Failed to initialize GLFW");
    }
}

/* CLEANUP */
void Engine::Cleanup( WindowManager *windowManager, VulkanManager *vulkanManager, ImGuiManager *imGuiManager) {

    glfwDestroyWindow( windowManager->window );
    glfwTerminate();



    delete windowManager;
    delete vulkanManager;
    //delete imGuiManager;
}
