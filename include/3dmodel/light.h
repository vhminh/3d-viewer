#pragma once

#include <glm/glm.hpp>

struct LightColor {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	LightColor(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
};

struct Attenuation {
	float constant;
	float linear;
	float quadratic;

	Attenuation(float constant, float linear, float quadratic);
};

struct DirectionalLight {
	LightColor color;
	Attenuation attenuation;
	glm::vec3 direction;

	DirectionalLight(LightColor color, Attenuation attenuation, glm::vec3 direction);
};

struct PointLight {
	LightColor color;
	Attenuation attenuation;
	glm::vec3 position;

	PointLight(LightColor color, Attenuation attenuation, glm::vec3 position);
};
