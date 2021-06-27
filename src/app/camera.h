#pragma once
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Camera {
public:
    Camera(glm::vec3 origin, glm::vec3 direction);
    const glm::vec3& get_origin() const;
    const glm::vec3& get_direction() const;
    glm::mat4 get_view_matrix() const;
    void process_input(GLFWwindow* window, float dt);
private:
    glm::vec3 origin;
    glm::vec3 direction;
};