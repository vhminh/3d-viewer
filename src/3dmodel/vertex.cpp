#include <3dmodel/vertex.h>

Vertex::Vertex(glm::vec3 position, glm::vec3 tangent, glm::vec3 bitangent, glm::vec3 normal,
               std::array<glm::vec2, MAX_NUM_UV_CHANNELS> tex_coords)
	: position(position), tangent(tangent), bitangent(bitangent), normal(normal), tex_coords(tex_coords) {}
