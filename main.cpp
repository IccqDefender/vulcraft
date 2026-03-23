#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <set>

int main() {
    glfwInit();

    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Vulcraft", nullptr, nullptr);

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

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

    VkInstance instance;
    vkCreateInstance(&createInfo, nullptr, &instance);
    VkSurfaceKHR surface;
    glfwCreateWindowSurface(instance, window, nullptr, &surface);

    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, physicalDevices.data());

    VkPhysicalDevice physicalDevice = physicalDevices.data()[0];

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilyProperties.data());

    uint32_t graphicsQueueFamilyIndex = 0;
    uint32_t presentQueueFamilyIndex = 0;

    for (size_t i = 0; i < queueFamilyProperties.size(); i++) {
        VkQueueFamilyProperties queueFamily = queueFamilyProperties[i];

        VkBool32 presentSupport = VK_FALSE;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);

        if (presentSupport) {
            presentQueueFamilyIndex = i;
        }

        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            graphicsQueueFamilyIndex = i;
            break;
        }
    }

    VkPhysicalDeviceFeatures deviceFeatures = {};
    float queuePriority = 1.0f;

    std::vector<VkDeviceQueueCreateInfo> deviceQueueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {
        graphicsQueueFamilyIndex,
        presentQueueFamilyIndex
    };

    for (const uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        deviceQueueCreateInfos.push_back(queueCreateInfo);
    }

    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

    deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(deviceQueueCreateInfos.size());
    deviceCreateInfo.pQueueCreateInfos = deviceQueueCreateInfos.data();

    VkDevice device;
    vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device);

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);
    std::vector<VkPresentModeKHR> presentModes(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.data());

    VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
    for (VkPresentModeKHR availableMode : presentModes) {
        if (availableMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            presentMode = availableMode;
            break;
        }
    }

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
    std::vector<VkSurfaceFormatKHR> swapChainFormates(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, swapChainFormates.data());

    VkSurfaceFormatKHR surfaceFormat = swapChainFormates[0];

    for (VkSurfaceFormatKHR availableFormat : swapChainFormates) {
        if (availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR &&
            availableFormat.format == VK_FORMAT_B8G8R8_SRGB) {
            surfaceFormat = availableFormat;
            break;
            }
    }

    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilities);
    VkExtent2D extent = surfaceCapabilities.currentExtent;

    uint32_t imageCount = surfaceCapabilities.minImageCount + 1;
    if (surfaceCapabilities.maxImageCount > 0 &&
    imageCount > surfaceCapabilities.maxImageCount) {
        imageCount = surfaceCapabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR swapChainCreateInfo = {};
    swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapChainCreateInfo.surface = surface;
    swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapChainCreateInfo.clipped = VK_TRUE;
    swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;
    swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapChainCreateInfo.imageArrayLayers = 1;

    uint32_t queueFamilyIndexes[] = { graphicsQueueFamilyIndex, presentQueueFamilyIndex };
    if (graphicsQueueFamilyIndex != presentQueueFamilyIndex) {
        swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapChainCreateInfo.queueFamilyIndexCount = 2;
        swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndexes;
    }else {
        swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    swapChainCreateInfo.presentMode = presentMode;
    swapChainCreateInfo.imageExtent = extent;
    swapChainCreateInfo.minImageCount = imageCount;
    swapChainCreateInfo.preTransform = surfaceCapabilities.currentTransform;
    swapChainCreateInfo.imageFormat = surfaceFormat.format;
    swapChainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;

    VkSwapchainKHR swapChain;
    vkCreateSwapchainKHR(device, &swapChainCreateInfo, nullptr, &swapChain);

    uint32_t swapChainImageCount;
    vkGetSwapchainImagesKHR(device, swapChain, &swapChainImageCount, nullptr);
    std::vector<VkImage> swapChainImages(swapChainImageCount);
    vkGetSwapchainImagesKHR(device, swapChain, &swapChainImageCount, swapChainImages.data());

    VkFormat swapChainImageFormat = surfaceFormat.format;
    VkExtent2D swapChainExtent = extent;
    std::vector<VkImageView> swapChainImageViews(swapChainImages.size());

    for (size_t i = 0; i < swapChainImages.size(); i++) {
        VkImageViewCreateInfo imageViewCreateInfo = {};
        imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewCreateInfo.image = swapChainImages[i];
        imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imageViewCreateInfo.format = swapChainImageFormat;
        imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
        imageViewCreateInfo.subresourceRange.levelCount = 1;
        imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        imageViewCreateInfo.subresourceRange.layerCount = 1;
        vkCreateImageView(device, &imageViewCreateInfo, nullptr, &swapChainImageViews[i]);

    }

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    return 0;
}