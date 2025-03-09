#include "3dmodel/light.h"

Attenuation::Attenuation(float constant, float linear, float quadratic)
	: constant(constant), linear(linear), quadratic(quadratic) {}

DirectionalLight::DirectionalLight(glm::vec3 color, glm::vec3 direction) : color(color), direction(direction) {}

PointLight::PointLight(glm::vec3 color, Attenuation attenuation, glm::vec3 position)
	: color(color), attenuation(attenuation), position(position) {}
