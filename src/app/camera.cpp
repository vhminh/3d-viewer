#include "app/camera.h"

#include "app/config.h"
#include "util/math.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(GLFWwindow* window, glm::vec3 origin, float yaw, float pitch)
	: window(window), origin(origin), yaw(yaw), pitch(pitch),
	  has_last_cursor(false) {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

const glm::vec3& Camera::get_origin() const { return origin; }

const glm::vec3 Camera::get_direction() const {
	float x = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	float y = sin(glm::radians(pitch));
	float z = -cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	return glm::vec3(x, y, z);
}

glm::mat4 Camera::get_view_matrix() const {
	return glm::lookAt(origin, origin + get_direction(),
	                   glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::process_input(float dt) {
	process_keyboard_input(dt);
	process_mouse_input(dt);
}

void Camera::process_keyboard_input(float dt) {
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 control_dir = glm::vec3(0.0f, 0.0f, 0.0f);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS ||
	    glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		control_dir.z -= 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS ||
	    glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		control_dir.z += 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS ||
	    glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		control_dir.x -= 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS ||
	    glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		control_dir.x += 1.0f;
	}
	if (glm::length(control_dir) < EPSILON) {
		// no control input
		return;
	}
	control_dir = glm::normalize(control_dir);
	glm::vec3 direction = get_direction();
	glm::vec3 move_vec = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 facing_dir_2D = glm::vec3(
		direction.x, 0.0, direction.z); // facing direction without the y-axis
	if (glm::length(facing_dir_2D) < EPSILON) {
		// looking at the sky or the ground
		return;
	}
	facing_dir_2D = glm::normalize(facing_dir_2D);
	move_vec += glm::normalize(glm::cross(facing_dir_2D, up)) * control_dir.x *
	            CAMERA_SPEED * dt;
	move_vec += -facing_dir_2D * control_dir.z * CAMERA_SPEED * dt;
	origin += move_vec;
}

void Camera::process_mouse_input(float dt) {
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	glm::dvec2 cur_cursor_pos = glm::dvec2(x, y);
	if (!has_last_cursor) {
		has_last_cursor = true;
		last_cursor_pos = cur_cursor_pos;
		return;
	}

	glm::dvec2 diff = cur_cursor_pos - last_cursor_pos;
	yaw += CAMERA_ROTATION_SPEED * diff.x;
	pitch -= CAMERA_ROTATION_SPEED * diff.y;
	pitch = clamp(pitch, -45.0, 45.0);

	last_cursor_pos = cur_cursor_pos;
}
