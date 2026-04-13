#ifndef VULCRAFT_WINDOW_H
#define VULCRAFT_WINDOW_H

#include <GLFW/glfw3.h>

class window {
    public:
    void CreateWindow(uint32_t width, uint32_t height, const char* title);
    void DestroyWindow();
    void CleanupWindow();

    [[nodiscard]] GLFWwindow* GetCurrentWindow() const { return m_window; };
    [[nodiscard]] bool ShouldClose() const { return glfwWindowShouldClose(m_window); }

    private:
    GLFWwindow* m_window = nullptr;
};



#endif //VULCRAFT_WINDOW_H
