#pragma once

#include <array>
#include <glm/glm.hpp>

#ifndef MAX_NUM_UV_CHANNELS
#define MAX_NUM_UV_CHANNELS 8
#endif

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	std::array<glm::vec2, MAX_NUM_UV_CHANNELS> tex_coords;
	Vertex(glm::vec3 position, glm::vec3 normal, std::array<glm::vec2, MAX_NUM_UV_CHANNELS> tex_coords);
};
