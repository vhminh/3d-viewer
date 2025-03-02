#include <3dmodel/vertex.h>

Vertex::Vertex(glm::vec3 position, glm::vec3 normal, std::array<glm::vec2, MAX_NUM_UV_CHANNELS> tex_coords)
	: position(position), normal(normal), tex_coords(tex_coords) {}
