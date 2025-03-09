#pragma once

#include <array>
#include <glm/glm.hpp>

#ifndef MAX_NUM_UV_CHANNELS
#define MAX_NUM_UV_CHANNELS 2 // AI_MAX_NUMBER_OF_TEXTURECOORDS is 8, but we only support 2
#endif

struct Vertex {
	glm::vec3 position;
	glm::vec3 tangent;
	glm::vec3 bitangent;
	glm::vec3 normal;
	std::array<glm::vec2, MAX_NUM_UV_CHANNELS> tex_coords;
	Vertex(glm::vec3 position, glm::vec3 tangent, glm::vec3 bitangent, glm::vec3 normal,
	       std::array<glm::vec2, MAX_NUM_UV_CHANNELS> tex_coords);
};
