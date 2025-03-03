#pragma once

#include "3dmodel/material.h"
#include "3dmodel/mesh.h"
#include "app/camera.h"
#include "glwrapper/shader.h"
#include "glwrapper/texture.h"
#include "util/macro.h"

#include <assimp/Importer.hpp>
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
	std::vector<Texture> textures;

	Mesh create_mesh(const aiScene* scene, const aiMesh* mesh);
	Material create_material(const aiScene* scene, const aiMaterial* material);
	Texture* load_texture(const aiScene* scene, TextureType type, const char* rel_path);
};
