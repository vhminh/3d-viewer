#pragma once

#include <glm/glm.hpp>

#ifndef MAX_NUM_UV_CHANNELS
#define MAX_NUM_UV_CHANNELS 8
#endif

struct Vertex {
	glm::vec3 position;
	glm::vec3 tangent;
	glm::vec3 bitangent;
	glm::vec3 normal;
	glm::vec2 tex_coord; // TODO: multiple uv channels
	Vertex(glm::vec3 position, glm::vec3 tangent, glm::vec3 bitangent, glm::vec3 normal, glm::vec2 tex_coord);
};
