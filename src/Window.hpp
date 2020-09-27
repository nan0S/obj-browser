#pragma once

#include <GLFW/glfw3.h>
#include <iostream>

class Window
{
public:
    static GLFWwindow* window;
    static int width, height;

    static bool create(const int& width, const int& height,
                       const std::string& title);

    static GLFWwindow* const get();
    static const int& getWidth();
    static const int& getHeight();
    static float getAspectRatio();

    static void resizeHandler(GLFWwindow* window, int w, int h);
    static bool pressed(GLuint key);
    static bool shouldClose();
    static void clear();
    static void display();
};