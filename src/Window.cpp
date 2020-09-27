#include "Window.hpp"

GLFWwindow* Window::window = nullptr;
int Window::width = 0;
int Window::height = 0;

bool Window::create(const int& width, const int& height,
                    const std::string& title)
{
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    Window::width = width;
    Window::height = height;

    window = glfwCreateWindow(width, height, title.c_str(),
                              NULL, NULL);
    if (window == NULL)
        return false;

    glfwMakeContextCurrent(window);
    // glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glClearColor(0, 0, 0, 1);
    glfwSetFramebufferSizeCallback(window, resizeHandler);

    return true;
}

GLFWwindow* const Window::get() { return window; }
const int& Window::getWidth() { return width; }
const int& Window::getHeight() { return height; }
float Window::getAspectRatio() { return float(width) / height; }

void Window::resizeHandler(GLFWwindow* window, int width, int height)
{
    Window::width = width;
    Window::height = height;
    glViewport(0, 0, width, height);
}

bool Window::pressed(GLuint key) { return glfwGetKey(window, key) == GLFW_PRESS; }
bool Window::shouldClose() { return glfwWindowShouldClose(window) || pressed(GLFW_KEY_ESCAPE); }
void Window::clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }
void Window::display() { glfwSwapBuffers(window); }

