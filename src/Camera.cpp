#include "Camera.hpp"

glm::mat4 Camera::projection = glm::mat4();
glm::mat4 Camera::view = glm::mat4();
glm::vec3 Camera::position = glm::vec3(0.f, 0.f, 5.f);
glm::vec3 Camera::direction = glm::vec3(0.f, 0.f, -1.f);
glm::vec3 Camera::up = glm::vec3(0.f, 1.f, 0.f);

float Camera::max_dist = 0.f;

void Camera::init(const float& max_dist)
{
    glfwSetScrollCallback(Window::get(), scrollHandler);

    Camera::max_dist = max_dist;
    compute();
}

void Camera::compute()
{
    float length = glm::length(position);
    projection = glm::perspective(
        glm::radians(45.f), 
        Window::getAspectRatio(),
        std::max(0.01f, length - max_dist), length + max_dist
    );

    view = glm::lookAt(
        position,
        position + direction,
        up
    );
}

glm::mat4 Camera::getMVP(const glm::mat4& model)
{
    return projection * view * model;
}

const glm::mat4& Camera::getView()
{
    return view;
}

const glm::vec3& Camera::getPosition()
{
    return position;
}

void Camera::zoom(const float& delta)
{
    position.z -= delta;
    compute();
}

void Camera::scrollHandler(GLFWwindow* window, double xoff, double yoff)
{
    if (yoff < 0)
        position.z *= 1.05f;
    else
        position.z *= 0.95f;

    compute();
}