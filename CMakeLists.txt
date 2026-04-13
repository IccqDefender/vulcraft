cmake_minimum_required(VERSION 3.20)
project(vulcraft)

set(CMAKE_CXX_STANDARD 17)

find_package(Vulkan REQUIRED)

include(FetchContent)

# ---------------- GLFW ----------------
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.3.9
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# ---------------- GLM ----------------
FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)

# ---------------- STB (textures) ----------------
FetchContent_Declare(
        stb
        GIT_REPOSITORY https://github.com/nothings/stb.git
        GIT_TAG master
)
FetchContent_MakeAvailable(stb)

# ---------------- ImGui ----------------
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG docking
)
FetchContent_MakeAvailable(imgui)

# ---------------- Shader compiler ----------------
find_program(GLSLC glslc)

if(NOT GLSLC)
    message(FATAL_ERROR "glslc not found! Install shaderc")
endif()

file(GLOB SHADERS "assets/shaders/*.vert" "assets/shaders/*.frag")

foreach(shader ${SHADERS})
    get_filename_component(FILE_NAME ${shader} NAME)
    set(SPIRV "${CMAKE_BINARY_DIR}/shaders/${FILE_NAME}.spv")

    add_custom_command(
            OUTPUT ${SPIRV}
            COMMAND ${GLSLC} ${shader} -o ${SPIRV}
            DEPENDS ${shader}
    )

    list(APPEND SPIRV_SHADERS ${SPIRV})
endforeach()

add_custom_target(Shaders ALL DEPENDS ${SPIRV_SHADERS})

# ---------------- Executable ----------------
add_executable(vulcraft src/main.cpp
        src/core/vk.cpp
        src/core/vk.h
        src/core/window.cpp
        src/core/window.h
        src/utils/Logger.cpp
        src/utils/Logger.h
        src/utils/LoggerMacros.h
        src/utils/Debug.h
        src/renderer/Swapchain.h
        src/renderer/Pipeline.h
        src/renderer/Buffer.h
        src/resources/ShaderLoader.h
        src/resources/TextureLoader.h
        src/renderer/Renderer.cpp
        src/renderer/Renderer.h
        src/math/Camera.h
        src/math/Transform.h
        src/systems/InputSystem.h
        src/renderer/vulkan/VulkanContext.cpp
        src/renderer/vulkan/VulkanContext.h
)

add_dependencies(vulcraft Shaders)

# ---------------- ImGui sources ----------------
set(IMGUI_DIR ${imgui_SOURCE_DIR})

target_sources(vulcraft PRIVATE
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp
)

# ---------------- Includes ----------------
target_include_directories(vulcraft PRIVATE
        ${IMGUI_DIR}
        ${IMGUI_DIR}/backends
        ${stb_SOURCE_DIR}
)

# ---------------- Linking ----------------
target_link_libraries(vulcraft PRIVATE
        Vulkan::Vulkan
        glfw
        glm
)

# ---------------- Warnings ----------------
target_compile_options(vulcraft PRIVATE -Wall -Wextra -Wpedantic)