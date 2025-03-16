#include "3dmodel/mesh.h"

#include "gl.h"
#include "gl3.h"
#include "utils/util.h"

#include <cstdio>
#include <glm/ext/matrix_clip_space.hpp>

Mesh::Mesh(glm::mat4 transform, std::vector<Vertex>&& vertices, std::vector<GLuint>&& indices, PBRMaterial&& material) :
	transform(transform), vertices(std::move(vertices)), indices(std::move(indices)), material(std::move(material)) {

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
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glVertexAttribPointer(
		4, 2 * MAX_NUM_UV_CHANNELS, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords)
	);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
}

Mesh::Mesh(Mesh&& another) :
	transform(another.transform), vertices(std::move(another.vertices)), indices(std::move(another.indices)),
	material(std::move(another.material)), vb(another.vb), va(another.va), eb(another.eb) {
	another.va = GLObject::ID_NONE;
	another.vb = GLObject::ID_NONE;
	another.eb = GLObject::ID_NONE;
}

void set_mesh_local_transform(Shader& shader, glm::mat4 local_transform) {
	shader.setUniformMat4("model_mat", local_transform);
}

void bind_material(Shader& shader, const PBRMaterial& material) {
	unsigned slot = 0;
	// base color
	if (material.base_color.has_value()) {
		auto tex = material.base_color.value();
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, tex->get_id());
		shader.setUniformTexture("base_color_texture", slot);
		shader.setUniformInt("base_color_uv_channel", material.base_color_uv_channel);
		shader.setUniformBool("has_base_color_texture", true);
		++slot;
	} else {
		shader.setUniformBool("has_base_color_texture", false);
	}
	shader.setUniformVec4("base_color_factor", material.base_color_factor);
	// normals
	if (material.normals.has_value()) {
		auto tex = material.normals.value();
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, tex->get_id());
		shader.setUniformTexture("normal_texture", slot);
		shader.setUniformInt("normal_uv_channel", material.normals_uv_channel);
		shader.setUniformBool("has_normal_texture", true);
		++slot;
	} else {
		shader.setUniformBool("has_normal_texture", false);
	}
	// metallic
	if (material.metallic.has_value()) {
		auto tex = material.metallic.value();
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, tex->get_id());
		shader.setUniformTexture("metallic_texture", slot);
		shader.setUniformInt("metallic_uv_channel", material.metallic_uv_channel);
		shader.setUniformBool("has_metallic_texture", true);
		++slot;
	} else {
		shader.setUniformBool("has_metallic_texture", false);
	}
	shader.setUniformFloat("metallic_factor", material.metallic_factor);
	// roughnes
	if (material.roughness.has_value()) {
		auto tex = material.roughness.value();
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, tex->get_id());
		shader.setUniformTexture("roughness_texture", slot);
		shader.setUniformInt("roughness_uv_channel", material.roughness_uv_channel);
		shader.setUniformBool("has_roughness_texture", true);
		++slot;
	} else {
		shader.setUniformBool("has_roughness_texture", false);
	}
	shader.setUniformFloat("roughness_factor", material.roughness_factor);
	// ambient occlusion
	if (material.roughness.has_value()) {
		auto tex = material.roughness.value();
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, tex->get_id());
		shader.setUniformTexture("ambient_occlusion_texture", slot);
		shader.setUniformInt("ao_uv_channel", material.ao_uv_channel);
		shader.setUniformBool("has_ao_texture", true);
		++slot;
	} else {
		shader.setUniformBool("has_ao_texture", false);
	}
	shader.setUniformFloat("ao_strength", material.ao_strength);
}

void Mesh::render(Shader& shader) const {
	set_mesh_local_transform(shader, transform);
	bind_material(shader, material);

	glBindVertexArray(va);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0); // unbind
}
