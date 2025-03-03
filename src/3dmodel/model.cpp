#include "3dmodel/model.h"

#include "3dmodel/mesh.h"
#include "assimp/material.h"
#include "assimp/types.h"

#include <OpenGL/gl.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <iostream>
#include <vector>

Vertex create_vertex(const aiVector3D& position, const aiVector3D& normal,
                     const std::array<glm::vec2, MAX_NUM_UV_CHANNELS> tex_coords);

glm::vec3 to_vec3(const aiColor3D& color) { return glm::vec3(color.r, color.g, color.b); }

glm::vec3 to_vec3(const aiVector3D& vec) { return glm::vec3(vec.x, vec.y, vec.z); }

std::string directory_of(const char* path) {
	std::string directory = std::string(path);
	directory.erase(directory.find_last_of('/'));
	return directory;
}

Model::Model(const char* path) : directory(directory_of(path)) {
	const aiScene* scene =
		importer.ReadFile(path, aiProcess_CalcTangentSpace | aiProcess_FlipUVs | aiProcess_Triangulate |
	                                aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);

	std::stack<aiNode*, std::vector<aiNode*>> stack;
	stack.push(scene->mRootNode);
	while (stack.size()) {
		aiNode* node = stack.top();
		stack.pop();

		for (unsigned i = 0; i < node->mNumMeshes; ++i) {
			unsigned mesh_idx = node->mMeshes[i];
			this->meshes.emplace_back(create_mesh(scene, scene->mMeshes[mesh_idx]));
		}

		for (int i = 0; i < node->mNumChildren; ++i) {
			stack.push(node->mChildren[i]);
		}
	}

	std::cout << "lights: " << scene->HasLights() << " " << scene->mNumLights << std::endl;
	std::cout << "cameras: " << scene->HasCameras() << " " << scene->mNumCameras << std::endl;
	std::cout << "materials: " << scene->HasMaterials() << " " << scene->mNumMaterials << std::endl;
	std::cout << "meshes: " << scene->HasMeshes() << " " << scene->mNumMeshes << std::endl;
	std::cout << "animations: " << scene->HasAnimations() << " " << scene->mNumAnimations << std::endl;
}

Mesh Model::create_mesh(const aiScene* scene, const aiMesh* mesh) {
	std::vector<Vertex> vertices;
	vertices.reserve(mesh->mNumVertices);
	for (int i = 0; i < mesh->mNumVertices; ++i) {
		std::array<glm::vec2, AI_MAX_NUMBER_OF_TEXTURECOORDS> tex_coords;
		for (int j = 0; j < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++j) {
			aiVector3D coord = mesh->mTextureCoords[/* j */ 0][i]; // TODO: multi uv channels
			tex_coords[j] = glm::vec2(coord.x, coord.y);
		}
		vertices.emplace_back(create_vertex(mesh->mVertices[i], mesh->mNormals[i], tex_coords));
	}

	std::vector<GLuint> indices;
	indices.reserve(mesh->mNumFaces * 3);
	for (int i = 0; i < mesh->mNumFaces; ++i) {
		aiFace& face = mesh->mFaces[i];
		if (face.mNumIndices != 3) {
			// not a triangle
			continue;
		}
		for (int i = 0; i < 3; ++i) {
			unsigned int index = face.mIndices[i];
			indices.push_back(index);
		}
	}
	indices.shrink_to_fit();

	Material material = create_material(scene, scene->mMaterials[mesh->mMaterialIndex]);
	return Mesh(std::move(vertices), std::move(indices), std::move(material));
}

Vertex create_vertex(const aiVector3D& position, const aiVector3D& normal,
                     const std::array<glm::vec2, MAX_NUM_UV_CHANNELS> tex_coords) {
	return Vertex(to_vec3(position), to_vec3(normal), tex_coords[0]); // TODO: multiple uv channels
}

std::ostream& operator<<(std::ostream& os, const aiColor3D& color) {
	os << "(" << color.r << "," << color.g << "," << color.b << ")";
	return os;
}

TextureType from_ai_texture_type(const aiTextureType ai_type) {
	if (ai_type == aiTextureType_NORMALS) {
		return TextureType::NORMALS;
	} else if (ai_type == aiTextureType_AMBIENT) {
		return TextureType::AMBIENT;
	} else if (ai_type == aiTextureType_DIFFUSE) {
		return TextureType::DIFFUSE;
	} else if (ai_type == aiTextureType_SPECULAR) {
		return TextureType::SPECULAR;
	}
	std::cerr << "unsupported assimp texture type " << ai_type << std::endl;
	assert(false);
}

aiTextureType to_ai_texture_type(const TextureType type) {
	if (type == TextureType::NORMALS) {
		return aiTextureType_NORMALS;
	} else if (type == TextureType::AMBIENT) {
		return aiTextureType_AMBIENT;
	} else if (type == TextureType::DIFFUSE) {
		return aiTextureType_DIFFUSE;
	} else if (type == TextureType::SPECULAR) {
		return aiTextureType_SPECULAR;
	}
	std::cerr << "unsupported texture type " << type << std::endl;
	assert(false);
}

Material Model::create_material(const aiScene* scene, const aiMaterial* material) {
	aiColor3D ambient, diffuse, specular;
	material->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
	material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
	material->Get(AI_MATKEY_COLOR_SPECULAR, specular);
	float shininess = 0, shininess_strength = 1.0;
	material->Get(AI_MATKEY_SHININESS, shininess);
	material->Get(AI_MATKEY_SHININESS_STRENGTH, shininess_strength);
	std::cout << "ambient: " << ambient << ", diffuse: " << diffuse << ", specular: " << specular
			  << ", shininess: " << shininess << ", shininess_strength: " << shininess_strength << std::endl;

	std::vector<Texture*> textures;
	for (int i = 0; i < TextureType::COUNT; ++i) {
		TextureType type = TextureType(i);
		aiTextureType ai_type = to_ai_texture_type(type);
		unsigned count = material->GetTextureCount(ai_type);
		for (int j = 0; j < count; ++j) {
			aiString rel_path; // path relative to main model file
			material->GetTexture(ai_type, j, &rel_path);
			textures.push_back(this->load_texture(scene, type, rel_path.C_Str()));
		}
	}

	return Material(to_vec3(ambient), to_vec3(diffuse), to_vec3(specular), shininess, shininess_strength, textures);
}

Texture* Model::load_texture(const aiScene* scene, TextureType type, const char* rel_path) {
	std::string path = directory + "/" + rel_path;
	for (int i = 0; i < this->textures.size(); ++i) {
		if (path == this->textures[i].get_path()) {
			return &this->textures[i];
		}
	}
	std::cerr << "load texture " << type << " " << rel_path << std::endl;
	const aiTexture* embedded_texture = scene->GetEmbeddedTexture(rel_path);
	if (embedded_texture) {
		std::cerr << "TODO: load embedded texture " << type << " " << rel_path << std::endl;
		return nullptr;
	}
	Texture texture = Texture::create(path.c_str(), type);
	return &this->textures.emplace_back(std::move(texture));
}

void Model::render(Shader& shader, const Camera& camera) const {
	for (const Mesh& mesh : this->meshes) {
		mesh.render(shader, camera);
	}
}
