cmake_minimum_required(VERSION 4.1)
project(vulcraft)

set(CMAKE_CXX_STANDARD 17)

set(IMGUI_DIR external/imgui-1.92.6-docking)

# Создаем статическую библиотеку из исходников ImGui
add_library(imgui STATIC
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
)

# Добавляем папку с заголовками
target_include_directories(imgui PUBLIC ${IMGUI_DIR})

find_package(Vulkan REQUIRED)

add_subdirectory(external/glfw)

add_executable(vulcraft main.cpp
        src/Managers/WindowManager/WindowManager.cpp
        src/Managers/WindowManager/WindowManager.h
        src/Engine.cpp
        src/Engine.h

)

target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan glfw imgui)

target_include_directories(${PROJECT_NAME} PRIVATE external/glfw/include)