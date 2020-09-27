#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Window.hpp"

class Camera
{
private:
    static glm::mat4 projection;
    static glm::mat4 view;

    static glm::vec3 position;
    static glm::vec3 direction;
    static glm::vec3 up;

    static float max_dist;

    static void compute();

public:
    static void init(const float& max_dist);
    static glm::mat4 getMVP(const glm::mat4& model);
    static const glm::mat4& getView();
    static const glm::vec3& getPosition();
    static void zoom(const float& zoom);    
    static void scrollHandler(GLFWwindow* window, double xoff, double yoff);
};