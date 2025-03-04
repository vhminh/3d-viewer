#include "3dmodel/light.h"

LightColor::LightColor(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
	: ambient(ambient), diffuse(diffuse), specular(specular) {}

Attenuation::Attenuation(float constant, float linear, float quadratic)
	: constant(constant), linear(linear), quadratic(quadratic) {}

DirectionalLight::DirectionalLight(LightColor color, Attenuation attenuation, glm::vec3 direction)
	: color(color), attenuation(attenuation), direction(direction) {}

PointLight::PointLight(LightColor color, Attenuation attenuation, glm::vec3 position)
	: color(color), attenuation(attenuation), position(position) {}
