cmake_minimum_required(VERSION 4.1)
project(vulcraft)

set(CMAKE_CXX_STANDARD 17)

find_package(Vulkan REQUIRED)

add_subdirectory(external/glfw)

add_executable(vulcraft main.cpp
        src/Managers/WindowManager/WindowManager.cpp
        src/Managers/WindowManager/WindowManager.h
        src/Engine.cpp
        src/Engine.h

)

target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan glfw)