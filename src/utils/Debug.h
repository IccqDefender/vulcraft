#pragma once

#include "Logger.h"
#include <string>
#include <vulkan/vulkan.h>

#define ASSERT(x) \
    do { \
        if (!(x)) { \
            Logger::Log(Logger::Level::Critical, Logger::Tag::Core, \
            std::string("ASSERT FAILED: ") + #x + \
            std::string("\nFile: ") + __FILE__ + \
            std::string("\nLine: ") + std::to_string(__LINE__)); \
            __builtin_trap(); \
        } \
    } while (0)

inline const char* VkResultToString(VkResult result) {
    switch (result) {
        case VK_SUCCESS: return "VK_SUCCESS";
        case VK_NOT_READY: return "VK_NOT_READY";
        case VK_TIMEOUT: return "VK_TIMEOUT";
        case VK_EVENT_SET: return "VK_EVENT_SET";
        case VK_EVENT_RESET: return "VK_EVENT_RESET";
        case VK_INCOMPLETE: return "VK_INCOMPLETE";

        case VK_ERROR_OUT_OF_HOST_MEMORY: return "VK_ERROR_OUT_OF_HOST_MEMORY";
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
        case VK_ERROR_INITIALIZATION_FAILED: return "VK_ERROR_INITIALIZATION_FAILED";
        case VK_ERROR_DEVICE_LOST: return "VK_ERROR_DEVICE_LOST";
        case VK_ERROR_MEMORY_MAP_FAILED: return "VK_ERROR_MEMORY_MAP_FAILED";
        case VK_ERROR_LAYER_NOT_PRESENT: return "VK_ERROR_LAYER_NOT_PRESENT";
        case VK_ERROR_EXTENSION_NOT_PRESENT: return "VK_ERROR_EXTENSION_NOT_PRESENT";
        case VK_ERROR_FEATURE_NOT_PRESENT: return "VK_ERROR_FEATURE_NOT_PRESENT";
        case VK_ERROR_INCOMPATIBLE_DRIVER: return "VK_ERROR_INCOMPATIBLE_DRIVER";
        case VK_ERROR_OUT_OF_DATE_KHR: return "VK_ERROR_OUT_OF_DATE_KHR";
        case VK_SUBOPTIMAL_KHR: return "VK_SUBOPTIMAL_KHR";

        default: return "UNKNOWN_VK_RESULT";
    }
}

#define VK_CHECK(x) \
    do { \
        VkResult result = (x); \
        if (result != VK_SUCCESS) { \
            Logger::Log(Logger::Level::Error, Logger::Tag::Vulkan, \
                std::string("Vulkan Error: ") + VkResultToString(result) + \
                std::string("\nFile: ") + __FILE__ + \
                std::string("\nLine: ") + std::to_string(__LINE__)); \
            __builtin_trap(); \
        } \
    } while (0)

#define DEBUG_BREAK() __builtin_trap()