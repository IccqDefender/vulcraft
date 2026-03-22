#include "Engine.h"

/* RUN ENGINE */
void Engine::Run() {

    InitGLFW(); // INIT GLFW

    glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );

    auto* _windowManager = new WindowManager();
    _windowManager->InitializeWindow();

    glfwMakeContextCurrent( _windowManager->window ); // MAKE CONTEXT WINDOW

    InitVulkan(_windowManager);

    /* MAIN LOOP */
    MainLoop(_windowManager);

    /* CLEANUP */
    Cleanup(_windowManager);
}

/* VULKAN FUNCTIONS*/
void Engine::InitVulkan( WindowManager *windowManager ) {
    VulkanCreateInstance();
    CreateWindowSurface(windowManager);
    FindPhysicalDevice();
    CheckSwapChainSupport();
    FindQueueFamilies();
    CreateLogicalDevice();
    CreateSemaphores();
    CreateCommandPool();
}

void Engine::VulkanCreateInstance() {
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulcraft";
    appInfo.applicationVersion = VK_MAKE_VERSION( 1, 0, 0 );
    appInfo.engineVersion = VK_MAKE_VERSION( 1, 0, 0 );
    appInfo.apiVersion = VK_API_VERSION_1_0;

    unsigned int m_glfwExtensionCount;
    const char** m_glfwExtensions;
    m_glfwExtensions = glfwGetRequiredInstanceExtensions(&m_glfwExtensionCount);

    std::vector<const char*> extensions;
    for (size_t i = 0; i < m_glfwExtensionCount; i++) {
        extensions.push_back(m_glfwExtensions[i]);
    }

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    if (extensionCount == 0) {
        std::cerr << "[VULKAN]: No extension supported" << std::endl;
        exit(-1);
    }

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = (uint32_t) extensions.size();
    createInfo.ppEnabledExtensionNames = extensions.data();

    if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS) {
        std::cerr << "[VULKAN]: Failed to create Vulkan instance" << std::endl;
        exit(-1);
    }else {
        std::cout << "[VULKAN]: Vulkan instance created" << std::endl;
    }
}

void Engine::CreateWindowSurface( WindowManager *windowManager ) {
    if (glfwCreateWindowSurface(m_instance, windowManager->window, nullptr, &m_windowSurface) != VK_SUCCESS) {
        std::cerr << "[VULKAN]: Failed to create window surface" << std::endl;
        exit(-1);
    }

    std::cout << "[VULKAN]: Window surface created" << std::endl;
}

void Engine::FindPhysicalDevice() {
    uint32_t m_deviceCount = 0;
    if (vkEnumeratePhysicalDevices(m_instance, &m_deviceCount, nullptr) != VK_SUCCESS || m_deviceCount == 0) {
        std::cerr << "[VULKAN]: Failed to get physical devices!" << std::endl;
    }

    m_deviceCount = 1;
    VkResult result = vkEnumeratePhysicalDevices(m_instance, &m_deviceCount, &m_physicalDevice);
    if (result != VK_SUCCESS && result != VK_INCOMPLETE) {
        std::cerr << "[VULKAN]: Enumerating physical devices failed!" << std::endl;
        exit(-1);
    }

    if (m_deviceCount == 0) {
        std::cerr << "[VULKAN]: No supported Vulkan physical device found!" << std::endl;
        exit(-1);
    }

    std::cout << "[VULKAN]: Physical device found" << std::endl;

    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(m_physicalDevice, &deviceProperties);
    vkGetPhysicalDeviceFeatures(m_physicalDevice, &deviceFeatures);

    uint32_t supportedVersion[] = {
        VK_VERSION_MAJOR(deviceProperties.apiVersion),
        VK_VERSION_MINOR(deviceProperties.apiVersion),
        VK_VERSION_PATCH(deviceProperties.apiVersion)
    };

    std::cout << "[VULKAN]: Physical device supports version " << supportedVersion[0] << "." << supportedVersion[1] << "." << supportedVersion[2] << std::endl;
}

void Engine::CheckSwapChainSupport() {
    uint32_t extensionCount = 0;
    vkEnumerateDeviceExtensionProperties(m_physicalDevice, nullptr, &extensionCount, nullptr);

    if (extensionCount == 0) {
        std::cerr << "[VULKAN]: Physical device doesn't support any extensions" << std::endl;
        exit(1);
    }

    std::vector<VkExtensionProperties> deviceExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(m_physicalDevice, nullptr, &extensionCount, deviceExtensions.data());

    for (const auto& extension : deviceExtensions) {
        if (strcmp(extension.extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0) {
            std::cout << "[VULKAN]: Physical device supports swap chains" << std::endl;
            return;
        }
    }

    std::cerr << "[VULKAN]: Physical device doesn't support swap chains" << std::endl;
    exit(1);
}

void Engine::FindQueueFamilies() {
    // Check queue families
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, nullptr);

    if (queueFamilyCount == 0) {
        std::cout << "[VULKAN]: Physical device has no queue families!" << std::endl;
        exit(1);
    }

    // Find queue family with graphics support
    // Note: is a transfer queue necessary to copy vertices to the gpu or can a graphics queue handle that?
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, queueFamilies.data());

    std::cout << "[VULKAN]: Physical device has " << queueFamilyCount << " queue families" << std::endl;

    bool foundGraphicsQueueFamily = false;
    bool foundPresentQueueFamily = false;

    for (uint32_t i = 0; i < queueFamilyCount; i++) {
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(m_physicalDevice, i, m_windowSurface, &presentSupport);

        if (queueFamilies[i].queueCount > 0 && queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            m_graphicsQueueFamily = i;
            foundGraphicsQueueFamily = true;

            if (presentSupport) {
                m_presentQueueFamily = i;
                foundPresentQueueFamily = true;
                break;
            }
        }

        if (!foundPresentQueueFamily && presentSupport) {
            m_presentQueueFamily = i;
            foundPresentQueueFamily = true;
        }
    }

    if (foundGraphicsQueueFamily) {
        std::cout << "[VULKAN]: Queue family #" << m_graphicsQueueFamily << " supports graphics" << std::endl;

        if (foundPresentQueueFamily) {
            std::cout << "[VULKAN]: Queue family #" << m_presentQueueFamily << " supports presentation" << std::endl;
        } else {
            std::cerr << "[VULKAN]: Could not find a valid queue family with present support" << std::endl;
            exit(1);
        }
    } else {
        std::cerr << "[VULKAN]: Could not find a valid queue family with graphics support" << std::endl;
        exit(1);
    }
}

void Engine::CreateLogicalDevice() {
    // Greate one graphics queue and optionally a separate presentation queue
		float queuePriority = 1.0f;

		VkDeviceQueueCreateInfo queueCreateInfo[2] = {};

		queueCreateInfo[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo[0].queueFamilyIndex = m_graphicsQueueFamily;
		queueCreateInfo[0].queueCount = 1;
		queueCreateInfo[0].pQueuePriorities = &queuePriority;

		queueCreateInfo[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo[0].queueFamilyIndex = m_presentQueueFamily;
		queueCreateInfo[0].queueCount = 1;
		queueCreateInfo[0].pQueuePriorities = &queuePriority;

		// Create logical device from physical device
		// Note: there are separate instance and device extensions!
		VkDeviceCreateInfo deviceCreateInfo = {};
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfo.pQueueCreateInfos = queueCreateInfo;

		if (m_graphicsQueueFamily == m_presentQueueFamily) {
			deviceCreateInfo.queueCreateInfoCount = 1;
		} else {
			deviceCreateInfo.queueCreateInfoCount = 2;
		}

		// Necessary for shader (for some reason)
		VkPhysicalDeviceFeatures enabledFeatures = {};
		enabledFeatures.shaderClipDistance = VK_TRUE;
		enabledFeatures.shaderCullDistance = VK_TRUE;

		const char* deviceExtensions = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
		deviceCreateInfo.enabledExtensionCount = 1;
		deviceCreateInfo.ppEnabledExtensionNames = &deviceExtensions;
		deviceCreateInfo.pEnabledFeatures = &enabledFeatures;

		if (vkCreateDevice(m_physicalDevice, &deviceCreateInfo, nullptr, &m_device) != VK_SUCCESS) {
			std::cerr << "[VULKAN]: Failed to create logical device" << std::endl;
			exit(1);
		}

		std::cout << "[VULKAN]: Created logical device" << std::endl;

		// Get graphics and presentation queues (which may be the same)
		vkGetDeviceQueue(m_device, m_graphicsQueueFamily, 0, &m_graphicsQueue);
		vkGetDeviceQueue(m_device, m_presentQueueFamily, 0, &m_presentQueue);

		std::cout << "[VULKAN]: Acquired graphics and presentation queues" << std::endl;

		vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &m_deviceMemoryProperties);
}

void Engine::CreateSemaphores() {
    VkSemaphoreCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    if (vkCreateSemaphore(m_device, &createInfo, nullptr, &m_imageAvailableSemaphore) != VK_SUCCESS ||
        vkCreateSemaphore(m_device, &createInfo, nullptr, &m_renderingFinishedSemaphore) != VK_SUCCESS) {
        std::cerr << "[VULKAN]: Failed to create semaphores" << std::endl;
        exit(1);
        } else {
            std::cout << "[VULKAN]: Created semaphores" << std::endl;
        }
}

void Engine::CreateCommandPool() {
    // Create graphics command pool
    VkCommandPoolCreateInfo poolCreateInfo = {};
    poolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolCreateInfo.queueFamilyIndex = m_graphicsQueueFamily;

    if (vkCreateCommandPool(m_device, &poolCreateInfo, nullptr, &m_commandPool) != VK_SUCCESS) {
        std::cerr << "[VULKAN]: Failed to create command queue for graphics queue family" << std::endl;
        exit(1);
    } else {
        std::cout << "[VULKAN]: Created command pool for graphics queue family" << std::endl;
    }
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
        throw std::runtime_error("[GLFW]: Failed to initialize GLFW");
    }
}

/* CLEANUP */
void Engine::Cleanup( WindowManager *windowManager ) {

    glfwDestroyWindow( windowManager->window );
    glfwTerminate();

    delete windowManager;
}
