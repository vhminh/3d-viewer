#pragma once

#include "3dmodel/light.h"
#include "3dmodel/material.h"
#include "3dmodel/vertex.h"
#include "gl.h"
#include "glwrapper/shader.h"
#include "utils/macro.h"

#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <vector>

class Mesh {
public:
	Mesh(glm::mat4 transform, std::vector<Vertex>&& vertices, std::vector<GLuint>&& indices, PBRMaterial&& material);
	NO_COPY(Mesh);
	MOVE(Mesh);

	void render(Shader& shader) const;

private:
	glm::mat4 transform; // transformation relative to world
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	PBRMaterial material;
	GLuint vb, va, eb;
};
