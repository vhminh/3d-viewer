#pragma once

#include <glm/glm.hpp>

struct Attenuation {
	float constant;
	float linear;
	float quadratic;

	Attenuation(float constant, float linear, float quadratic);
};

struct DirectionalLight {
	glm::vec3 color;
	glm::vec3 direction;

	DirectionalLight(glm::vec3 color, glm::vec3 direction);
};

struct PointLight {
	glm::vec3 color;
	Attenuation attenuation;
	glm::vec3 position;

	PointLight(glm::vec3 color, Attenuation attenuation, glm::vec3 position);
};
