#ifndef VULCRAFT_WINDOWMANAGER_H
#define VULCRAFT_WINDOWMANAGER_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace vulcraft {
    class WindowManager {
        public:
        WindowManager();
        ~WindowManager();

        void CreateWindow(uint32_t width, uint32_t height, char* title);

        bool IsWindowShouldClose(GLFWwindow* window) { return glfwWindowShouldClose(window); };
        GLFWwindow* GetWindow() { return m_window; };

        private:
        GLFWwindow* m_window = nullptr;

    };
} // vulcraft

#endif //VULCRAFT_WINDOWMANAGER_H