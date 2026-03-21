#include "Engine.h"

/* RUN ENGINE */
void Engine::Run() {

    InitGLFW(); // INIT GLFW

    glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );

    auto* _windowManager = new WindowManager();
    _windowManager->InitializeWindow();

    glfwMakeContextCurrent( _windowManager->window ); // MAKE CONTEXT WINDOW

    InitVulkan();

    /* MAIN LOOP */
    MainLoop(_windowManager);

    /* CLEANUP */
    Cleanup(_windowManager);
}

/* MAIN LOOP */
void Engine::MainLoop( WindowManager *windowManager ) {
    while (!glfwWindowShouldClose( windowManager->window ) ) {
        glfwPollEvents();
        glfwSwapBuffers( windowManager->window );
    }
}

/* INIT GLFW */
void Engine::InitGLFW() {
    glfwInit();
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }
}

/* INIT VULKAN */
void Engine::InitVulkan() {
    VulkanInstantiate();
}

/* VULKAN INSTANTIATE */
void Engine::VulkanInstantiate() {
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulcraft";
    appInfo.applicationVersion = VK_MAKE_VERSION( 1, 0, 0 );
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION( 1, 0, 0 );
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    uint32_t glfwExtensionCount = 0;
    const char** m_glfwExtensions;

    m_glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = m_glfwExtensions;

    createInfo.enabledLayerCount = 0;

    if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Vulkan instance");
    }

}

void Engine::CreateSurface( WindowManager *windowManager ) {
    if (glfwCreateWindowSurface(m_instance, windowManager->window, nullptr, nullptr) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Vulkan surface");
    }
}

/* CLEANUP */
void Engine::Cleanup( WindowManager *windowManager ) {
    vkDestroyInstance( m_instance, nullptr );

    glfwDestroyWindow( windowManager->window );
    glfwTerminate();

    delete windowManager;
}
