#include "3dmodel/material.h"

Material::Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, float shininess_strength)
	: ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess),
	  shininess_strength(shininess_strength) {}

