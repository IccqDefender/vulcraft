//
// Created by user on 21.03.2026.
//

#ifndef VULCRAFT_WINDOWMANAGER_H
#define VULCRAFT_WINDOWMANAGER_H

#include <GLFW/glfw3.h>
#include <iostream>


class WindowManager {
    public:
    void CreateWindow(); // VOID FUNCTION TO CREATE A WINDOW

    GLFWwindow* window;

    private:
    /* WINDOW SETTINGS */

    uint32_t WINDOW_WIDTH = 800; // WINDOW WIDTH
    uint32_t WINDOW_HEIGHT = 600; // WINDOW HEIGHT

    std::string WINDOW_TITLE = "Vulcraft"; // WINDOW TITLE

};


#endif //VULCRAFT_WINDOWMANAGER_H