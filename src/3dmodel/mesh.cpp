#include "3dmodel/mesh.h"

#include "app/config.h"
#include "gl3.h"
#include "util/util.h"

#include <GL/gl.h>
#include <GLES3/gl3.h>
#include <glm/ext/matrix_clip_space.hpp>

Mesh::Mesh(std::vector<Vertex>&& vertices, std::vector<GLuint>&& indices)
	: vertices(std::move(vertices)), indices(std::move(indices)) {
	glGenBuffers(1, &vb);
	glGenVertexArrays(1, &va);
	glGenBuffers(1, &eb);

	// vertex data
	glBindVertexArray(va);
	glBindBuffer(GL_ARRAY_BUFFER, vb);
	glBufferData(GL_ARRAY_BUFFER, sizeof_vec_data(vertices), vertices.data(), GL_STATIC_DRAW);

	// indices data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eb);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof_vec_data(vertices), indices.data(), GL_STATIC_DRAW);

	// attrib pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
}

Mesh::Mesh(Mesh&& another) : vertices(std::move(another.vertices)), indices(std::move(another.indices)) {}

void Mesh::render(Shader& shader, const Camera& camera) const {
	shader.use();

	shader.setUniformMat4("model_mat", glm::mat4(1.0));
	shader.setUniformMat4("view_mat", camera.get_view_matrix());
	const glm::mat4 projection_mat =
		glm::perspective(glm::radians(45.0), DEFAULT_WINDOW_WIDTH * 1.0 / DEFAULT_WINDOW_HEIGHT, 0.1, 100.0);
	shader.setUniformMat4("projection_mat", projection_mat);

	glBindVertexArray(va);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eb);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0); // unbind
}
