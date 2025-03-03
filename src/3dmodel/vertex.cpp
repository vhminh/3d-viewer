#include <3dmodel/vertex.h>

Vertex::Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 tex_coord)
	: position(position), normal(normal), tex_coord(tex_coord) {}
