#pragma once

#include "3dmodel/vertex.h"
#include "app/camera.h"
#include "glwrapper/shader.h"
#include "util/macro.h"

#include <GL/gl.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <vector>

class Mesh {
public:
	Mesh(std::vector<Vertex>&& vertices, std::vector<GLuint>&& indices);
	NO_COPY(Mesh);
	MOVE(Mesh);

	void render(Shader& shader, const Camera& camera) const;

private:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	GLuint vb, va, eb;
};
