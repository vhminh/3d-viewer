#pragma once

#include "3dmodel/model.h"
#include "app/ResourceManager.h"
#include "app/camera.h"
#include "glwrapper/shader.h"
#include "utils/macro.h"

#include <assimp/Importer.hpp>
#include <vector>

class Scene {
public:
	Scene(const std::vector<const char*>& paths);
	NO_COPY(Scene);
	NO_MOVE(Scene);

	void render(Shader& shader, const Camera& camera) const;

private:
	ResourceManager resource_manager;
	std::vector<Model> models;

	// accumulated lights from all models
	std::vector<DirectionalLight> directional_lights;
	std::vector<PointLight> point_lights;
};
