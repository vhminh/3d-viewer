#pragma once

#include "3dmodel/material.h"
#include "3dmodel/vertex.h"
#include "app/camera.h"
#include "gl.h"
#include "glwrapper/shader.h"
#include "util/macro.h"

#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <vector>

class Mesh {
public:
	Mesh(glm::mat4 transform, std::vector<Vertex>&& vertices, std::vector<GLuint>&& indices, Material&& material);
	NO_COPY(Mesh);
	MOVE(Mesh);

	void render(Shader& shader, const Camera& camera) const;

private:
	glm::mat4 transform; // transformation relative to world
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	Material material;
	GLuint vb, va, eb;
};
