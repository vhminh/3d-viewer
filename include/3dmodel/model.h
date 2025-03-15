#pragma once

#include "3dmodel/mesh.h"
#include "app/ResourceManager.h"

#include <string>
class Model {
public:
	Model(ResourceManager& resource_manager, const char* path);
	NO_COPY(Model);
	MOVE(Model);

	const std::vector<DirectionalLight>& get_directional_lights() const;
	const std::vector<PointLight>& get_point_lights() const;

	void render(Shader& shader, const Camera& camera, const std::vector<DirectionalLight>& directional_lights,
	            const std::vector<PointLight>& point_lights) const;

private:
	const std::string directory;
	std::vector<Mesh> meshes;
	std::vector<PBRMaterial> materials;
	std::vector<DirectionalLight> directional_lights;
	std::vector<PointLight> point_lights;
};
