#pragma once

#include "glwrapper/texture.h"

#include <glm/glm.hpp>
#include <vector>

struct Material {
	Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, float shininess_strength,
	         std::vector<std::shared_ptr<Texture>> textures);

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
	float shininess_strength;
	std::vector<std::shared_ptr<Texture>> textures;
};
