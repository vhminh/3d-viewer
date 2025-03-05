#include "3dmodel/mesh.h"

#include "app/config.h"
#include "gl.h"
#include "gl3.h"
#include "util/util.h"

#include <cstdio>
#include <glm/ext/matrix_clip_space.hpp>
#include <iostream>

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
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coord));
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

void set_transformation_matrices(Shader& shader, glm::mat4 local_transform, const Camera& camera) {
	shader.setUniformMat4("model_mat", local_transform);
	shader.setUniformMat4("view_mat", camera.get_view_matrix());
	const glm::mat4 projection_mat =
		glm::perspective(glm::radians(45.0), DEFAULT_WINDOW_WIDTH * 1.0 / DEFAULT_WINDOW_HEIGHT, 0.1, 100.0);
	shader.setUniformMat4("projection_mat", projection_mat);
	shader.setUniformVec3("camera_position", camera.origin);
}

void bind_textures(Shader& shader, const std::vector<std::shared_ptr<Texture>>& textures) {
	int tex_slot = 0;
	int ambient_count = 0;
	int diffuse_count = 0;
	int specular_count = 0;
	for (const std::shared_ptr<Texture> texture : textures) {
		char uniform_name[32] = "";
		switch (texture->get_type()) {
		case TextureType::AMBIENT: {
			int n = snprintf(uniform_name, 30, "ambient_map_%d", ambient_count++);
			assert(n < 30);
			break;
		}
		case TextureType::DIFFUSE: {
			int n = snprintf(uniform_name, 30, "diffuse_map_%d", diffuse_count++);
			assert(n < 30);
			break;
		}
		case TextureType::SPECULAR: {
			int n = snprintf(uniform_name, 30, "specular_map_%d", specular_count++);
			assert(n < 30);
			break;
		}
		default: {
			std::cerr << "skip texture of type " << texture->get_type() << std::endl;
			break;
		}
		}
		if (strlen(uniform_name) > 0) {
			glActiveTexture(GL_TEXTURE0 + tex_slot);
			glBindTexture(GL_TEXTURE_2D, texture->get_id());
			shader.setUniformTexture(uniform_name, tex_slot);
			tex_slot++;
		}
	}
}

void Mesh::render(Shader& shader, const Camera& camera, const std::vector<DirectionalLight>& directional_lights,
                  const std::vector<PointLight>& point_lights) const {
	set_transformation_matrices(shader, transform, camera);
	bind_textures(shader, material.textures);

	glBindVertexArray(va);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0); // unbind
}
