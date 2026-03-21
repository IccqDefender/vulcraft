#include "Engine.h"

/* RUN ENGINE */
void Engine::Run() {

    InitGLFW(); // INIT GLFW

    glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );

    auto* _windowManager = new WindowManager();
    _windowManager->InitializeWindow();

    glfwMakeContextCurrent( _windowManager->window ); // MAKE CONTEXT WINDOW

    InitVulkan( _windowManager );

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
void Engine::InitVulkan(WindowManager* windowManager ) {
    VulkanInstantiate();
    CreateSurface( windowManager );
    PickPhysicalDevice();
    //CreateLogicalDevice();
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
    if (glfwCreateWindowSurface(m_instance, windowManager->window, nullptr, &m_surface) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Vulkan surface");
    }
}

void Engine::PickPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);
    if (deviceCount == 0) {
        throw std::runtime_error("Failed to find GPU's with Vulkan support");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());

    for (const auto& device : devices) {
        VkPhysicalDeviceProperties m_deviceProperties;
        vkGetPhysicalDeviceProperties(device, &m_deviceProperties);

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        bool foundGraphics = false;
        for (uint32_t i = 0; i < queueFamilyCount; i++) {
            if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                foundGraphics = true;
                break;
            }
        }
        if (foundGraphics) {
            m_physicalDevice = device;
            break;
        }
    }

    if (m_physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}

void Engine::CreateLogicalDevice() {
    uint32_t m_queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &m_queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(m_queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &m_queueFamilyCount, queueFamilies.data());

    uint32_t m_graphicsQueueFamily = 0;
    for (uint32_t i = 0; i < m_queueFamilyCount; i++) {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            m_graphicsQueueFamily = i;
            break;
        }
    }

    float m_queuePriority = 1.0f;
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = m_queueFamilyCount;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &m_queuePriority;

    VkPhysicalDeviceFeatures m_deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pEnabledFeatures = &m_deviceFeatures;

    const char* deviceExtensions[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    createInfo.enabledExtensionCount = 1;
    createInfo.ppEnabledExtensionNames = deviceExtensions;

    if (vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }

    vkGetDeviceQueue(m_device, m_graphicsQueueFamily, 0, &m_graphicsQueue);
}

/* CLEANUP */
void Engine::Cleanup( WindowManager *windowManager ) {
    vkDestroyInstance( m_instance, nullptr );

    glfwDestroyWindow( windowManager->window );
    glfwTerminate();

    delete windowManager;
}
