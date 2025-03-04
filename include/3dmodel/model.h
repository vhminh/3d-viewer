#pragma once

#include "3dmodel/material.h"
#include "3dmodel/mesh.h"
#include "app/camera.h"
#include "glwrapper/shader.h"
#include "glwrapper/texture.h"
#include "util/macro.h"

#include <assimp/Importer.hpp>
#include <map>
#include <vector>

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
	std::vector<Material> materials;
	std::vector<std::shared_ptr<Texture>> textures; // TODO: can hold `Texture`s in place
	std::map<std::string, std::shared_ptr<Texture>> texture_by_path;

	Mesh create_mesh(const aiScene* scene, const aiMesh* mesh, const glm::mat4& transform);
	Material create_material(const aiScene* scene, const aiMaterial* material);
	std::shared_ptr<Texture> load_texture(const aiScene* scene, TextureType type, const char* rel_path);
};
