#pragma once

#include "3dmodel/light.h"
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

	void render_opaque_meshes(Shader& shader) const;
	void render_semi_transparent_meshes(Shader& shader) const;

private:
	const std::string directory;
	std::vector<Mesh> meshes;
	std::vector<Mesh*> opaque_meshes;
	std::vector<Mesh*> semi_transparent_meshes;
	std::vector<PBRMaterial> materials;
	std::vector<DirectionalLight> directional_lights;
	std::vector<PointLight> point_lights;
};
