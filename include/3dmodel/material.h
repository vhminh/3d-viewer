#pragma once

#include <glm/glm.hpp>
struct Material {
	Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, float shininess_strength);

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
	float shininess_strength;
};
