#pragma once

#include "3dmodel/light.h"
#include "3dmodel/material.h"
#include "3dmodel/mesh.h"
#include "app/camera.h"
#include "glwrapper/shader.h"
#include "glwrapper/texture.h"
#include "util/macro.h"

#include <assimp/Importer.hpp>
#include <map>
#include <vector>

#ifndef MAX_NUM_DIRECTIONAL_LIGHTS
#define MAX_NUM_DIRECTIONAL_LIGHTS 8
#endif

#ifndef MAX_NUM_POINT_LIGHTS
#define MAX_NUM_POINT_LIGHTS 24
#endif

class Model {
public:
	Model(const char* path);
	NO_COPY(Model);
	NO_MOVE(Model);

	void render(Shader& shader, const Camera& camera) const;

private:
	const std::string directory;
	Assimp::Importer importer;
	std::vector<Mesh> meshes;
	std::vector<PBRMaterial> materials;
	std::vector<std::shared_ptr<Texture>> textures; // TODO: can hold `Texture`s in place
	std::map<std::string, std::shared_ptr<Texture>> texture_by_path;
	std::vector<DirectionalLight> directional_lights;
	std::vector<PointLight> point_lights;

	void load_meshes(const aiScene* scene);
	void load_lights(const aiScene* scene);
	Mesh load_mesh(const aiScene* scene, const aiMesh* mesh, const glm::mat4& transform);
	PBROptTex load_texure_opt(const aiScene* scene, const aiMaterial* material, TextureType type);
	PBRMaterial load_material(const aiScene* scene, const aiMaterial* material);

	PBRColorTex load_color_texture(const aiScene* scene, const aiMaterial* material, TextureType type,
	                               const char* fallback_matkey, unsigned fallback_matkey_type,
	                               unsigned fallback_matkey_idx);
	PBRPropTex load_float_texture(const aiScene* scene, const aiMaterial* material, TextureType type,
	                              const char* fallback_matkey, unsigned fallback_matkey_type,
	                              unsigned fallback_matkey_idx);
	PBRPropTex load_float_texture(const aiScene* scene, const aiMaterial* material, TextureType type, float fallback);
	std::shared_ptr<Texture> load_texture(const aiScene* scene, TextureType type, const char* rel_path);
};
