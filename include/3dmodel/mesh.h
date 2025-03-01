#pragma once

#include "3dmodel/vertex.h"
#include "app/camera.h"
#include "glwrapper/shader.h"
#include "util/macro.h"

#include <GL/gl.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <vector>

struct Mesh {
	Mesh(std::vector<Vertex>&& vertices, std::vector<GLuint>&& indices);
	NO_COPY(Mesh);
	MOVE(Mesh);
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	void render(Shader& shader, const Camera& camera) const;
};
