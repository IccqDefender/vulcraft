#ifndef VULCRAFT_WINDOWMANAGER_H
#define VULCRAFT_WINDOWMANAGER_H

#include <GLFW/glfw3.h>
#include <iostream>


class WindowManager {
    public:
    void InitializeWindow(int width, int height, std::string title); // VOID FUNCTION TO CREATE A WINDOW

    const uint32_t WINDOW_WIDTH = 800; // WINDOW WIDTH
    const uint32_t WINDOW_HEIGHT = 600; // WINDOW HEIGHT
    const std::string WINDOW_TITLE = "Vulcraft"; // WINDOW TITLE

    GLFWwindow* window;

    private:
    /* WINDOW SETTINGS */

};


#endif //VULCRAFT_WINDOWMANAGER_H