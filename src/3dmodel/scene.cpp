#include "3dmodel/scene.h"

#include "3dmodel/mesh.h"
#include "3dmodel/utils/assimp_conversions.h"
#include "app/config.h"
#include "assimp/light.h"
#include "assimp/material.h"
#include "assimp/types.h"
#include "gl3.h"

#include <array>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <cstdlib>
#include <cstring>
#include <glm/ext/matrix_clip_space.hpp>
#include <vector>

Vertex create_vertex(const aiVector3D& position, const aiVector3D& tangent, const aiVector3D& bitangent,
                     const aiVector3D& normal, const std::array<glm::vec2, MAX_NUM_UV_CHANNELS> tex_coords);

Scene::Scene(const std::vector<const char*>& paths) {
	for (const char* path : paths) {
		Model& model = models.emplace_back(resource_manager, path);
		auto directional_lights = model.get_directional_lights();
		auto point_lights = model.get_point_lights();
		this->directional_lights.insert(this->directional_lights.end(), directional_lights.begin(),
		                                directional_lights.end());
		this->point_lights.insert(this->point_lights.end(), point_lights.begin(), point_lights.end());
	}
}

void set_camera_view_transforms(Shader& shader, const Camera& camera) {
	shader.setUniformMat4("view_mat", camera.get_view_matrix());
	const glm::mat4 projection_mat =
		glm::perspective(glm::radians(45.0), DEFAULT_WINDOW_WIDTH * 1.0 / DEFAULT_WINDOW_HEIGHT, 0.1, 100.0);
	shader.setUniformMat4("projection_mat", projection_mat);
	shader.setUniformVec3("camera_position", camera.origin);
}

void set_light_uniforms(Shader& shader, const std::vector<DirectionalLight>& directional_lights,
                        const std::vector<PointLight>& point_lights) {
	shader.setUniformInt("num_directional_lights", directional_lights.size());
	shader.setUniformInt("num_point_lights", point_lights.size());
	char buf[64];
	for (int i = 0; i < directional_lights.size(); ++i) {
		const DirectionalLight& light = directional_lights[i];
		snprintf(buf, 63, "directional_lights[%d].color", i);
		shader.setUniformVec3(buf, light.color);
		snprintf(buf, 63, "directional_lights[%d].direction", i);
		shader.setUniformVec3(buf, (float*)&light.direction);
	}
	for (int i = 0; i < point_lights.size(); ++i) {
		const PointLight& light = point_lights[i];
		snprintf(buf, 63, "point_lights[%d].color", i);
		shader.setUniformVec3(buf, light.color);
		snprintf(buf, 63, "point_lights[%d].attenuation.constant", i);
		shader.setUniformFloat(buf, light.attenuation.constant);
		snprintf(buf, 63, "point_lights[%d].attenuation.linear", i);
		shader.setUniformFloat(buf, light.attenuation.linear);
		snprintf(buf, 63, "point_lights[%d].attenuation.quadratic", i);
		shader.setUniformFloat(buf, light.attenuation.quadratic);
		snprintf(buf, 63, "point_lights[%d].position", i);
		shader.setUniformVec3(buf, (float*)&light.position);
	}
}

void Scene::render(Shader& shader, const Camera& camera) const {
	shader.use();

	// TODO: make this a feature, not a hack lol
	glm::vec3 light_color = glm::vec3(0.8, 0.8, 0.8);
	PointLight player_light = PointLight(light_color, Attenuation(1.0, 0.14, 0.07),
	                                     glm::vec3(camera.origin.x, camera.origin.y + 1.0, camera.origin.z));

	std::vector<PointLight> point_lights = this->point_lights;
	point_lights.push_back(player_light);

	set_light_uniforms(shader, directional_lights, point_lights);
	set_camera_view_transforms(shader, camera);
	for (const Model& model : this->models) {
		model.render(shader, camera, directional_lights, point_lights);
	}
}
