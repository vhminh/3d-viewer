#pragma once
#include <tuple>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Camera {
public:
    Camera(GLFWwindow* window, glm::vec3 origin, float yaw, float pitch);
    const glm::vec3& get_origin() const;
    const glm::vec3 get_direction() const;
    glm::mat4 get_view_matrix() const;
    void process_input(float dt);
private:
    GLFWwindow* window;
    glm::vec3 origin;
    double pitch, yaw;
    glm::dvec2 last_cursor_pos;
    bool has_last_cursor;

    void process_keyboard_input(float dt);
    void process_mouse_input(float dt);
};
