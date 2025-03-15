#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <utils/macro.h>

struct Camera {
public:
	Camera(glm::vec3 origin, float yaw, float pitch);

	glm::vec3 origin;
	double pitch, yaw;

	const glm::vec3 get_direction() const;
	glm::mat4 get_view_matrix() const;
};
