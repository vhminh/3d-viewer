#include "3dmodel/mesh.h"

Mesh::Mesh(std::vector<Vertex>&& vertices, std::vector<GLuint>&& indices)
	: vertices(std::move(vertices)), indices(std::move(indices)) {}

Mesh::Mesh(Mesh&& another) : vertices(std::move(another.vertices)), indices(std::move(another.indices)) {}
