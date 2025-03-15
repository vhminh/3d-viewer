#include "3dmodel/mesh.h"

#include "app/config.h"
#include "gl.h"
#include "gl3.h"
#include "utils/util.h"

#include <cstdio>
#include <glm/ext/matrix_clip_space.hpp>
#include <iostream>

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
	if (std::holds_alternative<std::shared_ptr<Texture>>(material.albedo)) {
		auto tex = std::get<std::shared_ptr<Texture>>(material.albedo);
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, tex->get_id());
		shader.setUniformTexture("albedo_map", slot);
		shader.setUniformBool("use_albedo_map", true);
		shader.setUniformInt("albedo_uv_channel", material.albedo_uv_channel);
		++slot;
	} else {
		auto color = std::get<glm::vec3>(material.albedo);
		shader.setUniformVec3("albedo_color", color);
		shader.setUniformBool("use_albedo_map", false);
	}
	if (material.normals.has_value()) {
		auto tex = material.normals.value();
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, tex->get_id());
		shader.setUniformTexture("normal_map", slot);
		shader.setUniformBool("use_normal_map", true);
		shader.setUniformInt("normal_uv_channel", material.normals_uv_channel);
		++slot;
	} else {
		shader.setUniformBool("use_normal_map", false);
	}
	if (std::holds_alternative<std::shared_ptr<Texture>>(material.metallic)) {
		auto tex = std::get<std::shared_ptr<Texture>>(material.metallic);
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, tex->get_id());
		shader.setUniformTexture("metallic_map", slot);
		shader.setUniformBool("use_metallic_map", true);
		shader.setUniformInt("metallic_uv_channel", material.metallic_uv_channel);
		++slot;
	} else {
		float value = std::get<float>(material.metallic);
		shader.setUniformFloat("metallic_factor", value);
		shader.setUniformBool("use_metallic_map", false);
	}
	if (std::holds_alternative<std::shared_ptr<Texture>>(material.roughness)) {
		auto tex = std::get<std::shared_ptr<Texture>>(material.roughness);
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, tex->get_id());
		shader.setUniformTexture("roughness_map", slot);
		shader.setUniformBool("use_roughness_map", true);
		shader.setUniformInt("roughness_uv_channel", material.roughness_uv_channel);
		++slot;
	} else {
		float value = std::get<float>(material.roughness);
		shader.setUniformFloat("roughness_factor", value);
		shader.setUniformBool("use_roughness_map", false);
	}
	if (std::holds_alternative<std::shared_ptr<Texture>>(material.ambient_occlusion)) {
		auto tex = std::get<std::shared_ptr<Texture>>(material.ambient_occlusion);
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, tex->get_id());
		shader.setUniformTexture("ambient_occlusion_map", slot);
		shader.setUniformBool("use_ambient_occlusion_map", true);
		shader.setUniformInt("ao_uv_channel", material.ao_uv_channel);
		++slot;
	} else {
		float value = std::get<float>(material.ambient_occlusion);
		shader.setUniformFloat("ambient_occlusion_factor", value);
		shader.setUniformBool("use_ambient_occlusion_map", false);
	}
}

void Mesh::render(Shader& shader) const {
	set_mesh_local_transform(shader, transform);
	bind_material(shader, material);

	glBindVertexArray(va);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0); // unbind
}
