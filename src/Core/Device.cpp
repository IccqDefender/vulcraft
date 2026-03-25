#include "Device.h"
#define VK_CHECK(x) if ((x) != VK_SUCCESS) throw std::runtime_error("Vulkan error");

void Device::Init(GLFWwindow* window) {
    CreateInstance();
    CreateSurface(window);
}

void Device::CreateInstance() {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
    if (!glfwExtensions) {
        throw std::runtime_error("[VULKAN]: glfwExtensions not available");
    }

    VkApplicationInfo app_info = {};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = "Vulcraft";
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = nullptr;
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion = VK_API_VERSION_1_4;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &app_info;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    vkCreateInstance(&createInfo, nullptr, &m_instance);
    if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS) {
        throw std::runtime_error("[VULKAN]: Failed to create Vulkan instance!");
    }
}

void Device::CreateSurface(GLFWwindow* window) {
    glfwCreateWindowSurface(m_instance, window, nullptr, &m_surface);
}

void Device::PickPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);

    std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, physicalDevices.data());

    m_physicalDevice = physicalDevices.data()[0];
}

void Device::CreateLogicalDevice() {
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> families(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, families.data());

    for (uint32_t i = 0; i < families.size(); i++) {
        if (families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            m_graphicsQueueFamilyIndex = i;

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(m_physicalDevice, i, m_surface, &presentSupport);
        if (presentSupport)
            m_presentQueueFamilyIndex = i;
    }

    float priority = 1.0f;
    std::vector<VkDeviceQueueCreateInfo> queues;
    std::set<uint32_t> unique = {m_graphicsQueueFamilyIndex, m_presentQueueFamilyIndex};

    for (uint32_t q : unique) {
        VkDeviceQueueCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        info.queueFamilyIndex = q;
        info.queueCount = 1;
        info.pQueuePriorities = &priority;
        queues.push_back(info);
    }

    const std::vector<const char*> extensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = queues.size();
    createInfo.pQueueCreateInfos = queues.data();
    createInfo.enabledExtensionCount = extensions.size();
    createInfo.ppEnabledExtensionNames = extensions.data();

    VK_CHECK(vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device));

    VkQueue graphicQueue;
    VkQueue presentQueue;
    vkGetDeviceQueue(m_device, m_graphicsQueueFamilyIndex, 0, &graphicQueue);
    vkGetDeviceQueue(m_device, m_presentQueueFamilyIndex, 0, &presentQueue);
}

void Device::Cleanup() {
    vkDestroyDevice(m_device, nullptr);
    vkDestroyInstance(m_instance, nullptr);
    vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
}