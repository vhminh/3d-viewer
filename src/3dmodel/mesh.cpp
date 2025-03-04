#include "3dmodel/mesh.h"

#include "app/config.h"
#include "gl.h"
#include "gl3.h"
#include "util/util.h"

#include <glm/ext/matrix_clip_space.hpp>

Mesh::Mesh(glm::mat4 transform, std::vector<Vertex>&& vertices, std::vector<GLuint>&& indices, Material&& material)
	: transform(transform), vertices(std::move(vertices)), indices(std::move(indices)), material(std::move(material)) {

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
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coord));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}

Mesh::Mesh(Mesh&& another)
	: transform(another.transform), vertices(std::move(another.vertices)), indices(std::move(another.indices)),
	  material(std::move(another.material)), va(another.va), vb(another.vb), eb(another.eb) {
	another.va = GLObject::ID_NONE;
	another.vb = GLObject::ID_NONE;
	another.eb = GLObject::ID_NONE;
}

void Mesh::render(Shader& shader, const Camera& camera) const {
	shader.use();
	shader.setUniformMat4("model_mat", transform);
	shader.setUniformMat4("view_mat", camera.get_view_matrix());
	const glm::mat4 projection_mat =
		glm::perspective(glm::radians(45.0), DEFAULT_WINDOW_WIDTH * 1.0 / DEFAULT_WINDOW_HEIGHT, 0.1, 100.0);
	shader.setUniformMat4("projection_mat", projection_mat);

	GLenum err = 0;
	int tex_slot = 0;
	for (const std::shared_ptr<Texture> texture : material.textures) {
		// TODO: multi texture support
		switch (texture->get_type()) {
		case TextureType::NORMALS: {
			glActiveTexture(GL_TEXTURE0 + tex_slot);
			glBindTexture(GL_TEXTURE_2D, texture->get_id());
			shader.setUniformTexture("normal_map", tex_slot);
			break;
		}
		case TextureType::AMBIENT: {
			glActiveTexture(GL_TEXTURE0 + tex_slot);
			glBindTexture(GL_TEXTURE_2D, texture->get_id());
			shader.setUniformTexture("ambient_map_0", tex_slot);
			break;
		}
		case TextureType::DIFFUSE: {
			glActiveTexture(GL_TEXTURE0 + tex_slot);
			glBindTexture(GL_TEXTURE_2D, texture->get_id());
			shader.setUniformTexture("diffuse_map_0", tex_slot);
			break;
		}
		}
		tex_slot++;
	}

	glBindVertexArray(va);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0); // unbind
}
