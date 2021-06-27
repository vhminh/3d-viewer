#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "config.h"

Camera::Camera(glm::vec3 origin, glm::vec3 direction) : origin(origin), direction(direction) {}

const glm::vec3& Camera::get_origin() const {
    return origin;
}

const glm::vec3& Camera::get_direction() const {
    return direction;
}

glm::mat4 Camera::get_view_matrix() const {
    return glm::lookAt(origin, origin + direction, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::process_input(GLFWwindow* window, float dt) {
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    // move
    glm::vec3 dir = glm::vec3(0.0f, 0.0f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        dir.z -= 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        dir.z += 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        dir.x -= 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        dir.x += 1.0f;
    }
    if (glm::length(dir) > EPSILON) {
        dir = glm::normalize(dir);
        glm::vec3 move_vec = glm::vec3(0.0f, 0.0f, 0.0f);
        move_vec += glm::normalize(glm::cross(direction, up)) * dir.x * CAMERA_SPEED * dt;
        move_vec += -direction * dir.z * CAMERA_SPEED * dt;
        origin += move_vec;
    }

    // change facing dir horizontally
    glm::mat4 transform = glm::mat4(1.0f);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        transform = glm::rotate(transform, glm::radians(CAMERA_ROTATION_SPEED * dt), up);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        transform = glm::rotate(transform, glm::radians(CAMERA_ROTATION_SPEED * dt), -up);
    }
    direction = transform * glm::vec4(direction.x, direction.y, direction.z, 1.0f);
}

