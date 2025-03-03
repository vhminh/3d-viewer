#include "3dmodel/mesh.h"

#include "app/config.h"
#include "gl.h"
#include "gl3.h"
#include "util/util.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <iostream>

Mesh::Mesh(std::vector<Vertex>&& vertices, std::vector<GLuint>&& indices, Material&& material)
	: vertices(std::move(vertices)), indices(std::move(indices)), material(std::move(material)) {

	glGenVertexArrays(1, &va);
	glBindVertexArray(va);
	glGenBuffers(1, &vb);
	glGenBuffers(1, &eb);

	// vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vb);
	glBufferData(GL_ARRAY_BUFFER, sizeof_vec_data(this->vertices), this->vertices.data(), GL_STATIC_DRAW);

	// indices data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eb);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof_vec_data(this->indices), this->indices.data(), GL_STATIC_DRAW);

	// attrib pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glVertexAttribPointer(1, 2, GL_UNSIGNED_INT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coord));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}

Mesh::Mesh(Mesh&& another)
	: vertices(std::move(another.vertices)), indices(std::move(another.indices)), material(std::move(another.material)),
	  va(another.va), vb(another.vb), eb(another.eb) {
	another.va = GLObject::ID_NONE;
	another.vb = GLObject::ID_NONE;
	another.eb = GLObject::ID_NONE;
}

void Mesh::render(Shader& shader, const Camera& camera) const {
	shader.use();
	shader.setUniformMat4("model_mat", glm::mat4(1.0));
	shader.setUniformMat4("view_mat", camera.get_view_matrix());
	const glm::mat4 projection_mat =
		glm::perspective(glm::radians(45.0), DEFAULT_WINDOW_WIDTH * 1.0 / DEFAULT_WINDOW_HEIGHT, 0.1, 100.0);
	shader.setUniformMat4("projection_mat", projection_mat);

	glBindVertexArray(va);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0); // unbind
}
