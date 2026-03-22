#include "Engine.h"

struct Vertex {
	float pos[3];
	float color[3];
};

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
	CreateVertexBuffer();
	CreateUniformBuffer();
	CreateSwapChain(windowManager);
	CreateRenderPass();
	CreateImageViews();
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

// Find device memory that is supported by the requirements (typeBits) and meets the desired properties
VkBool32 Engine::GetMemoryType(uint32_t typeBits, VkFlags properties, uint32_t* typeIndex) {
	for (uint32_t i = 0; i < 32; i++) {
		if ((typeBits & 1) == 1) {
			if ((m_deviceMemoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				*typeIndex = i;
				return true;
			}
		}
		typeBits >>= 1;
	}
	return false;
}
void Engine::UpdateUniformData() {
	// Rotate based on time
	auto timeNow = std::chrono::high_resolution_clock::now();
	long long millis = std::chrono::duration_cast<std::chrono::milliseconds>(m_timeStart - timeNow).count();
	float angle = (millis % 4000) / 4000.0f * glm::radians(360.f);

	glm::mat4 modelMatrix;
	modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 0, 1));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f / 3.0f, -0.5f / 3.0f, 0.0f));

	// Set up view
	auto viewMatrix = glm::lookAt(glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), glm::vec3(0, 0, -1));

	// Set up projection
	auto projMatrix = glm::perspective(glm::radians(70.f), 800 / (float) 600, 0.1f, 10.0f); // FIX IT

	m_uniformBufferData.transformationMatrix = projMatrix * viewMatrix * modelMatrix;

	void* data;
	vkMapMemory(m_device, m_uniformBufferMemory, 0, sizeof(m_uniformBufferData), 0, &data);
	memcpy(data, &m_uniformBufferData, sizeof(m_uniformBufferData));
	vkUnmapMemory(m_device, m_uniformBufferMemory);
}

VkExtent2D Engine::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities, WindowManager* windowManager) {
	if (surfaceCapabilities.currentExtent.width == -1) {
		VkExtent2D swapChainExtent = {};

		swapChainExtent.width = std::min(std::max(windowManager->m_WINDOW_WIDTH, surfaceCapabilities.minImageExtent.width), surfaceCapabilities.maxImageExtent.width);
		swapChainExtent.height = std::min(std::max(windowManager->m_WINDOW_HEIGHT, surfaceCapabilities.minImageExtent.height), surfaceCapabilities.maxImageExtent.height);

		return swapChainExtent;
	} else {
		return surfaceCapabilities.currentExtent;
	}
}
VkPresentModeKHR Engine::ChoosePresentMode(const std::vector<VkPresentModeKHR> presentModes) {
	for (const auto& presentMode : presentModes) {
		if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			return presentMode;
		}
	}

	// If mailbox is unavailable, fall back to FIFO (guaranteed to be available)
	return VK_PRESENT_MODE_FIFO_KHR;
}
VkSurfaceFormatKHR Engine::ChooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
	// We can either choose any format
	if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
		return{ VK_FORMAT_R8G8B8A8_UNORM, VK_COLORSPACE_SRGB_NONLINEAR_KHR };
	}

	// Or go with the standard format - if available
	for (const auto& availableSurfaceFormat : availableFormats) {
		if (availableSurfaceFormat.format == VK_FORMAT_R8G8B8A8_UNORM) {
			return availableSurfaceFormat;
		}
	}

	// Or fall back to the first available one
	return availableFormats[0];
}

void Engine::CreateVertexBuffer() {
    // Setup vertices
		std::vector<Vertex> vertices = {
			{ { -0.5f, -0.5f,  0.0f }, { 1.0f, 0.0f, 0.0f } },
			{ { -0.5f,  0.5f,  0.0f }, { 0.0f, 1.0f, 0.0f } },
			{ {  0.5f,  0.5f,  0.0f }, { 0.0f, 0.0f, 1.0f } }
		};
		uint32_t verticesSize = (uint32_t) (vertices.size() * sizeof(vertices[0]));

		// Setup indices
		std::vector<uint32_t> indices = { 0, 1, 2 };
		uint32_t indicesSize = (uint32_t) (indices.size() * sizeof(indices[0]));

		VkMemoryAllocateInfo memAlloc = {};
		memAlloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		VkMemoryRequirements memReqs;
		void* data;

		struct StagingBuffer {
			VkDeviceMemory memory;
			VkBuffer buffer;
		};

		struct {
			StagingBuffer vertices;
			StagingBuffer indices;
		} stagingBuffers;

		// Allocate command buffer for copy operation
		VkCommandBufferAllocateInfo cmdBufInfo = {};
		cmdBufInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		cmdBufInfo.commandPool = m_commandPool;
		cmdBufInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		cmdBufInfo.commandBufferCount = 1;

		VkCommandBuffer copyCommandBuffer;
		vkAllocateCommandBuffers(m_device, &cmdBufInfo, &copyCommandBuffer);

		// First copy vertices to host accessible vertex buffer memory
		VkBufferCreateInfo vertexBufferInfo = {};
		vertexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		vertexBufferInfo.size = verticesSize;
		vertexBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

		vkCreateBuffer(m_device, &vertexBufferInfo, nullptr, &stagingBuffers.vertices.buffer);

		vkGetBufferMemoryRequirements(m_device, stagingBuffers.vertices.buffer, &memReqs);
		memAlloc.allocationSize = memReqs.size;
		GetMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, &memAlloc.memoryTypeIndex);
		vkAllocateMemory(m_device, &memAlloc, nullptr, &stagingBuffers.vertices.memory);

		vkMapMemory(m_device, stagingBuffers.vertices.memory, 0, verticesSize, 0, &data);
		memcpy(data, vertices.data(), verticesSize);
		vkUnmapMemory(m_device, stagingBuffers.vertices.memory);
		vkBindBufferMemory(m_device, stagingBuffers.vertices.buffer, stagingBuffers.vertices.memory, 0);

		// Then allocate a gpu only buffer for vertices
		vertexBufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		vkCreateBuffer(m_device, &vertexBufferInfo, nullptr, &m_vertexBuffer);
		vkGetBufferMemoryRequirements(m_device, m_vertexBuffer, &memReqs);
		memAlloc.allocationSize = memReqs.size;
		GetMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &memAlloc.memoryTypeIndex);
		vkAllocateMemory(m_device, &memAlloc, nullptr, &m_vertexBufferMemory);
		vkBindBufferMemory(m_device, m_vertexBuffer, m_vertexBufferMemory, 0);

		// Next copy indices to host accessible index buffer memory
		VkBufferCreateInfo indexBufferInfo = {};
		indexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		indexBufferInfo.size = indicesSize;
		indexBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

		vkCreateBuffer(m_device, &indexBufferInfo, nullptr, &stagingBuffers.indices.buffer);
		vkGetBufferMemoryRequirements(m_device, stagingBuffers.indices.buffer, &memReqs);
		memAlloc.allocationSize = memReqs.size;
		GetMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, &memAlloc.memoryTypeIndex);
		vkAllocateMemory(m_device, &memAlloc, nullptr, &stagingBuffers.indices.memory);
		vkMapMemory(m_device, stagingBuffers.indices.memory, 0, indicesSize, 0, &data);
		memcpy(data, indices.data(), indicesSize);
		vkUnmapMemory(m_device, stagingBuffers.indices.memory);
		vkBindBufferMemory(m_device, stagingBuffers.indices.buffer, stagingBuffers.indices.memory, 0);

		// And allocate another gpu only buffer for indices
		indexBufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		vkCreateBuffer(m_device, &indexBufferInfo, nullptr, &m_indexBuffer);
		vkGetBufferMemoryRequirements(m_device, m_indexBuffer, &memReqs);
		memAlloc.allocationSize = memReqs.size;
		GetMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &memAlloc.memoryTypeIndex);
		vkAllocateMemory(m_device, &memAlloc, nullptr, &m_indexBufferMemory);
		vkBindBufferMemory(m_device, m_indexBuffer, m_indexBufferMemory, 0);

		// Now copy data from host visible buffer to gpu only buffer
		VkCommandBufferBeginInfo bufferBeginInfo = {};
		bufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		bufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(copyCommandBuffer, &bufferBeginInfo);

		VkBufferCopy copyRegion = {};
		copyRegion.size = verticesSize;
		vkCmdCopyBuffer(copyCommandBuffer, stagingBuffers.vertices.buffer, m_vertexBuffer, 1, &copyRegion);
		copyRegion.size = indicesSize;
		vkCmdCopyBuffer(copyCommandBuffer, stagingBuffers.indices.buffer, m_indexBuffer, 1, &copyRegion);

		vkEndCommandBuffer(copyCommandBuffer);

		// Submit to queue
		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &copyCommandBuffer;

		vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(m_graphicsQueue);

		vkFreeCommandBuffers(m_device, m_commandPool, 1, &copyCommandBuffer);

		vkDestroyBuffer(m_device, stagingBuffers.vertices.buffer, nullptr);
		vkFreeMemory(m_device, stagingBuffers.vertices.memory, nullptr);
		vkDestroyBuffer(m_device, stagingBuffers.indices.buffer, nullptr);
		vkFreeMemory(m_device, stagingBuffers.indices.memory, nullptr);

		std::cout << "[VULKAN]: Set up vertex and index buffers" << std::endl;

		// Binding and attribute descriptions
		m_vertexBindingDescription.binding = 0;
		m_vertexBindingDescription.stride = sizeof(vertices[0]);
		m_vertexBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		// vec2 position
		m_vertexAttributeDescriptions.resize(2);
		m_vertexAttributeDescriptions[0].binding = 0;
		m_vertexAttributeDescriptions[0].location = 0;
		m_vertexAttributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;

		// vec3 color
		m_vertexAttributeDescriptions[1].binding = 0;
		m_vertexAttributeDescriptions[1].location = 1;
		m_vertexAttributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		m_vertexAttributeDescriptions[1].offset = sizeof(float) * 3;
}

void Engine::CreateUniformBuffer() {
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = sizeof(m_uniformBufferData);
	bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

	vkCreateBuffer(m_device, &bufferInfo, nullptr, &m_uniformBuffer);

	VkMemoryRequirements memReqs;
	vkGetBufferMemoryRequirements(m_device, m_uniformBuffer, &memReqs);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memReqs.size;
	GetMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, &allocInfo.memoryTypeIndex);

	vkAllocateMemory(m_device, &allocInfo, nullptr, &m_uniformBufferMemory);
	vkBindBufferMemory(m_device, m_uniformBuffer, m_uniformBufferMemory, 0);

	UpdateUniformData();
}

void Engine::CreateSwapChain(WindowManager *windowManager) {
	// Find surface capabilities
		VkSurfaceCapabilitiesKHR surfaceCapabilities;
		if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevice, m_windowSurface, &surfaceCapabilities) != VK_SUCCESS) {
			std::cerr << "[VULKAN]: Failed to acquire presentation surface capabilities" << std::endl;
			exit(1);
		}

		// Find supported surface formats
		uint32_t formatCount;
		if (vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_windowSurface, &formatCount, nullptr) != VK_SUCCESS || formatCount == 0) {
			std::cerr << "[VULKAN]: Failed to get number of supported surface formats" << std::endl;
			exit(1);
		}

		std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
		if (vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_windowSurface, &formatCount, surfaceFormats.data()) != VK_SUCCESS) {
			std::cerr << "[VULKAN]: Failed to get supported surface formats" << std::endl;
			exit(1);
		}

		// Find supported present modes
		uint32_t presentModeCount;
		if (vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, m_windowSurface, &presentModeCount, nullptr) != VK_SUCCESS || presentModeCount == 0) {
			std::cerr << "[VULKAN]: Failed to get number of supported presentation modes" << std::endl;
			exit(1);
		}

		std::vector<VkPresentModeKHR> presentModes(presentModeCount);
		if (vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, m_windowSurface, &presentModeCount, presentModes.data()) != VK_SUCCESS) {
			std::cerr << "[VULKAN]: Failed to get supported presentation modes" << std::endl;
			exit(1);
		}

		// Determine number of images for swap chain
		uint32_t imageCount = surfaceCapabilities.minImageCount + 1;
		if (surfaceCapabilities.maxImageCount != 0 && imageCount > surfaceCapabilities.maxImageCount) {
			imageCount = surfaceCapabilities.maxImageCount;
		}

		std::cout << "[VULKAN]: Using " << imageCount << " images for swap chain" << std::endl;

		// Select a surface format
		VkSurfaceFormatKHR surfaceFormat = ChooseSurfaceFormat(surfaceFormats);

		// Select swap chain size
		m_swapChainExtent = ChooseSwapExtent(surfaceCapabilities, windowManager);

		// Determine transformation to use (preferring no transform)
		VkSurfaceTransformFlagBitsKHR surfaceTransform;
		if (surfaceCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
			surfaceTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		} else {
			surfaceTransform = surfaceCapabilities.currentTransform;
		}

		// Choose presentation mode (preferring MAILBOX ~= triple buffering)
		VkPresentModeKHR presentMode = ChoosePresentMode(presentModes);

		// Finally, create the swap chain
		VkSwapchainCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = m_windowSurface;
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = m_swapChainExtent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;
		createInfo.pQueueFamilyIndices = nullptr;
		createInfo.preTransform = surfaceTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = m_oldSwapChain;

		if (vkCreateSwapchainKHR(m_device, &createInfo, nullptr, &m_swapChain) != VK_SUCCESS) {
			std::cerr << "[VULKAN]: Failed to create swap chain" << std::endl;
			exit(1);
		} else {
			std::cout << "[VULKAN]: Created swap chain" << std::endl;
		}

		if (m_oldSwapChain != VK_NULL_HANDLE) {
			vkDestroySwapchainKHR(m_device, m_oldSwapChain, nullptr);
		}
		m_oldSwapChain = m_swapChain;

		m_swapChainFormat = surfaceFormat.format;

		// Store the images used by the swap chain
		// Note: these are the images that swap chain image indices refer to
		// Note: actual number of images may differ from requested number, since it's a lower bound
		uint32_t actualImageCount = 0;
		if (vkGetSwapchainImagesKHR(m_device, m_swapChain, &actualImageCount, nullptr) != VK_SUCCESS || actualImageCount == 0) {
			std::cerr << "[VULKAN]: Failed to acquire number of swap chain images" << std::endl;
			exit(1);
		}

		m_swapChainImages.resize(actualImageCount);

		if (vkGetSwapchainImagesKHR(m_device, m_swapChain, &actualImageCount, m_swapChainImages.data()) != VK_SUCCESS) {
			std::cerr << "[VULKAN]: Failed to acquire swap chain images" << std::endl;
			exit(1);
		}

		std::cout << "[VULKAN]: Acquired swap chain images" << std::endl;
}

void Engine::CreateRenderPass() {
	VkAttachmentDescription attachmentDescription = {};
	attachmentDescription.format = m_swapChainFormat;
	attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
	attachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	// Note: hardware will automatically transition attachment to the specified layout
	// Note: index refers to attachment descriptions array
	VkAttachmentReference colorAttachmentReference = {};
	colorAttachmentReference.attachment = 0;
	colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	// Note: this is a description of how the attachments of the render pass will be used in this sub pass
	// e.g. if they will be read in shaders and/or drawn to
	VkSubpassDescription subPassDescription = {};
	subPassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subPassDescription.colorAttachmentCount = 1;
	subPassDescription.pColorAttachments = &colorAttachmentReference;

	// Create the render pass
	VkRenderPassCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	createInfo.attachmentCount = 1;
	createInfo.pAttachments = &attachmentDescription;
	createInfo.subpassCount = 1;
	createInfo.pSubpasses = &subPassDescription;

	if (vkCreateRenderPass(m_device, &createInfo, nullptr, &m_renderPass) != VK_SUCCESS) {
		std::cerr << "[VULKAN]: Failed to create render pass" << std::endl;
		exit(1);
	} else {
		std::cout << "[VULKAN]: Created render pass" << std::endl;
	}
}

void Engine::CreateImageViews() {
	m_swapChainImageViews.resize(m_swapChainImages.size());

	// Create an image view for every image in the swap chain
	for (size_t i = 0; i < m_swapChainImages.size(); i++) {
		VkImageViewCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = m_swapChainImages[i];
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = m_swapChainFormat;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(m_device, &createInfo, nullptr, &m_swapChainImageViews[i]) != VK_SUCCESS) {
			std::cerr << "[VULKAN]: Failed to create image view for swap chain image #" << i << std::endl;
			exit(1);
		}
	}

	std::cout << "[VULKAN]: Created image views for swap chain images" << std::endl;
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
