#pragma once

#include "3dmodel/vertex.h"
#include "util/macro.h"

#include <OpenGL/gltypes.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <vector>

struct Mesh {
	Mesh(std::vector<Vertex>&& vertices, std::vector<GLuint>&& indices);
	NO_COPY(Mesh);
	MOVE(Mesh);
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
};
