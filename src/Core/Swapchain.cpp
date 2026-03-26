//
// Created by user on 24.03.2026.
//

#include "Swapchain.h"


void Swapchain::InitSwapchain() {
    CreateSwapchain();
}

void Swapchain::Cleanup() {
    vkDestroySwapchainKHR(m_device, m_swapChain, nullptr);
}

void Swapchain::CreateSwapchain() {
    vkCreateSwapchainKHR(m_device, &m_swapChainCreateInfo, nullptr, &m_swapChain);
}

void Swapchain::CreateSwapchainInfo(VkSurfaceKHR surface) {
    m_swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    m_swapChainCreateInfo.surface = surface;
    m_swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    m_swapChainCreateInfo.clipped = VK_TRUE;
    m_swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;
    m_swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    m_swapChainCreateInfo.imageArrayLayers = 1;

    uint32_t queueFamilyIndexes[] = { m_graphicsQueueFamilyIndex, m_presentQueueFamilyIndex };
    if (m_graphicsQueueFamilyIndex != m_presentQueueFamilyIndex) {
        m_swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        m_swapChainCreateInfo.queueFamilyIndexCount = 2;
        m_swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndexes;
    }else {
        m_swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    m_swapChainCreateInfo.presentMode = presentMode;
    m_swapChainCreateInfo.imageExtent = extent;
    m_swapChainCreateInfo.minImageCount = imageCount;
    m_swapChainCreateInfo.preTransform = surfaceCapabilities.currentTransform;
    m_swapChainCreateInfo.imageFormat = surfaceFormat.format;
    m_swapChainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
}

