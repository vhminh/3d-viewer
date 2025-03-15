#include "app/camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 origin, float yaw, float pitch) : origin(origin), pitch(pitch), yaw(yaw) {}

const glm::vec3 Camera::get_direction() const {
	float x = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	float y = sin(glm::radians(pitch));
	float z = -cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	return glm::vec3(x, y, z);
}

glm::mat4 Camera::get_view_matrix() const {
	return glm::lookAt(origin, origin + get_direction(), glm::vec3(0.0f, 1.0f, 0.0f));
}
