#include "3dmodel/model.h"

#include "3dmodel/mesh.h"
#include "assimp/light.h"
#include "assimp/material.h"
#include "assimp/matrix4x4.h"
#include "assimp/types.h"
#include "gl3.h"

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <cstdlib>
#include <iostream>
#include <stack>
#include <vector>

Vertex create_vertex(const aiVector3D& position, const aiVector3D& normal,
                     const std::array<glm::vec2, MAX_NUM_UV_CHANNELS> tex_coords);

glm::vec3 to_vec3(const aiColor3D& color) { return glm::vec3(color.r, color.g, color.b); }

glm::vec3 to_vec3(const aiVector3D& vec) { return glm::vec3(vec.x, vec.y, vec.z); }

glm::mat4 to_mat4(const aiMatrix4x4& mat) {
	// clang-format off
	return glm::mat4(
		mat.a1, mat.b1, mat.c1, mat.d1,
		mat.a2, mat.b2, mat.c2, mat.d2,
		mat.a3, mat.b3, mat.c3, mat.d3,
		mat.a4, mat.b4, mat.c4, mat.d4
	);
	// clang-format on
}

std::string directory_of(const char* path) {
	std::string directory = std::string(path);
	directory.erase(directory.find_last_of('/'));
	return directory;
}

Model::Model(const char* path) : directory(directory_of(path)) {
	const aiScene* scene =
		importer.ReadFile(path, aiProcess_CalcTangentSpace | aiProcess_FlipUVs | aiProcess_Triangulate |
	                                aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);

	if (!scene) {
		std::cerr << "cannot load scene: " << importer.GetErrorString() << std::endl;
	}

	load_lights(scene);

	load_meshes(scene);

	std::cout << "lights: " << scene->HasLights() << " " << scene->mNumLights << std::endl;
	std::cout << "cameras: " << scene->HasCameras() << " " << scene->mNumCameras << std::endl;
	std::cout << "materials: " << scene->HasMaterials() << " " << scene->mNumMaterials << std::endl;
	std::cout << "meshes: " << scene->HasMeshes() << " " << scene->mNumMeshes << std::endl;
	std::cout << "animations: " << scene->HasAnimations() << " " << scene->mNumAnimations << std::endl;
}

void Model::load_meshes(const aiScene* scene) {
	std::stack<std::pair<aiNode*, glm::mat4>> stack;
	stack.push(std::make_pair(scene->mRootNode, glm::mat4(1.0)));
	while (stack.size()) {
		auto [node, transform] = stack.top();
		stack.pop();

		glm::mat4 new_transform = transform * to_mat4(node->mTransformation);

		for (unsigned i = 0; i < node->mNumMeshes; ++i) {
			unsigned mesh_idx = node->mMeshes[i];
			this->meshes.emplace_back(load_mesh(scene, scene->mMeshes[mesh_idx], new_transform));
		}

		for (int i = 0; i < node->mNumChildren; ++i) {
			stack.push(std::make_pair(node->mChildren[i], new_transform));
		}
	}
}

Mesh Model::load_mesh(const aiScene* scene, const aiMesh* mesh, const glm::mat4& transform) {
	std::vector<Vertex> vertices;
	vertices.reserve(mesh->mNumVertices);
	for (int i = 0; i < mesh->mNumVertices; ++i) {
		std::array<glm::vec2, AI_MAX_NUMBER_OF_TEXTURECOORDS> tex_coords;
		for (int j = 0; j < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++j) {
			if (mesh->mTextureCoords[j]) {
				aiVector3D coord = mesh->mTextureCoords[j][i]; // TODO: multi uv channels
				tex_coords[j] = glm::vec2(coord.x, coord.y);
				break; // TODO: multi uv channels
			}
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

	Material material = load_material(scene, scene->mMaterials[mesh->mMaterialIndex]);
	return Mesh(transform, std::move(vertices), std::move(indices), std::move(material));
}

Vertex create_vertex(const aiVector3D& position, const aiVector3D& normal,
                     const std::array<glm::vec2, MAX_NUM_UV_CHANNELS> tex_coords) {
	return Vertex(to_vec3(position), glm::normalize(to_vec3(normal)), tex_coords[0]); // TODO: multiple uv channels
}

std::ostream& operator<<(std::ostream& os, const aiColor3D& color) {
	os << "(" << color.r << "," << color.g << "," << color.b << ")";
	return os;
}

TextureType from_ai_texture_type(const aiTextureType ai_type) {
	if (ai_type == aiTextureType_AMBIENT) {
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
	if (type == TextureType::AMBIENT) {
		return aiTextureType_AMBIENT;
	} else if (type == TextureType::DIFFUSE) {
		return aiTextureType_DIFFUSE;
	} else if (type == TextureType::SPECULAR) {
		return aiTextureType_SPECULAR;
	}
	std::cerr << "unsupported texture type " << type << std::endl;
	assert(false);
}

Material Model::load_material(const aiScene* scene, const aiMaterial* material) {
	aiColor3D ambient, diffuse, specular;
	material->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
	material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
	material->Get(AI_MATKEY_COLOR_SPECULAR, specular);
	float shininess = 0, shininess_strength = 1.0;
	material->Get(AI_MATKEY_SHININESS, shininess);
	material->Get(AI_MATKEY_SHININESS_STRENGTH, shininess_strength);
	std::cout << "ambient: " << ambient << ", diffuse: " << diffuse << ", specular: " << specular
			  << ", shininess: " << shininess << ", shininess_strength: " << shininess_strength << std::endl;

	std::vector<std::shared_ptr<Texture>> textures;
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

std::tuple<std::vector<unsigned char>, int, int> texture_data(const aiTexture* tex) {
	if (tex->mHeight == 0) {
		std::cerr << "TODO: support compressed embedded texture" << std::endl;
		return std::make_tuple<std::vector<unsigned char>, int, int>(std::vector<unsigned char>(), 0, 0);
	} else {
		std::vector<unsigned char> data(tex->mWidth * tex->mHeight * 3);
		std::cerr << "TODO: fill data for embedded texture" << std::endl;
		return std::make_tuple(data, tex->mWidth, tex->mHeight);
	}
}

std::shared_ptr<Texture> Model::load_texture(const aiScene* scene, TextureType type, const char* rel_path) {
	std::string path = directory + "/" + rel_path;
	if (texture_by_path.contains(path)) {
		return texture_by_path.at(path);
	}
	std::cerr << "load texture " << type << " " << rel_path << std::endl;
	const aiTexture* embedded_texture = scene->GetEmbeddedTexture(rel_path);
	if (embedded_texture) {
		auto [data, w, h] = texture_data(embedded_texture);
		std::shared_ptr<Texture> ptr = std::make_shared<Texture>(Texture::create(data.data(), w, h, type));
		this->textures.push_back(ptr);
		texture_by_path[path] = ptr;
		return ptr;
	} else {
		std::shared_ptr<Texture> ptr = std::make_shared<Texture>(Texture::create(path.c_str(), type));
		this->textures.push_back(ptr);
		texture_by_path[path] = ptr;
		return ptr;
	}
}

void transforms_by_node_name_helper(const aiNode* node, const glm::mat4& prev_transform,
                                    std::map<std::string_view, std::vector<glm::mat4>>& result) {
	std::string_view node_name = std::string_view(node->mName.C_Str());
	glm::mat4 transform = prev_transform * to_mat4(node->mTransformation);
	if (node_name.size()) {
		result[node_name].push_back(prev_transform * to_mat4(node->mTransformation));
	}
	for (int i = 0; i < node->mNumChildren; ++i) {
		transforms_by_node_name_helper(node->mChildren[i], transform, result);
	}
}

std::map<std::string_view, std::vector<glm::mat4>> transforms_by_node_name(const aiScene* scene) {
	std::map<std::string_view, std::vector<glm::mat4>> result;
	transforms_by_node_name_helper(scene->mRootNode, glm::mat4(1.0), result);
	return result;
}

void Model::load_lights(const aiScene* scene) {
	std::map<std::string_view, std::vector<glm::mat4>> transforms_by_name = transforms_by_node_name(scene);
	for (int i = 0; i < scene->mNumLights; ++i) {
		aiLight* ai_light = scene->mLights[i];
		LightColor color = LightColor(to_vec3(ai_light->mColorAmbient), to_vec3(ai_light->mColorDiffuse),
		                              to_vec3(ai_light->mColorSpecular));
		Attenuation attenuation =
			Attenuation(ai_light->mAttenuationConstant, ai_light->mAttenuationLinear, ai_light->mAttenuationQuadratic);

		const std::vector<glm::mat4>& transforms = transforms_by_name[std::string_view(ai_light->mName.C_Str())];
		if (transforms.empty()) {
			std::cerr << "cannot find any nodes correspond to light source with name \"" << ai_light->mName.C_Str()
					  << "\"" << std::endl;
		}
		for (const glm::mat4& transform : transforms) {
			switch (ai_light->mType) {
			case aiLightSource_DIRECTIONAL: {
				glm::vec3 rel_dir = to_vec3(ai_light->mDirection);
				glm::vec4 rel_dir_v4 = glm::vec4(rel_dir.x, rel_dir.y, rel_dir.z, 0.0);
				glm::vec4 abs_dir_v4 = transform * rel_dir_v4;
				glm::vec3 abs_dir = glm::vec3(abs_dir_v4.x, abs_dir_v4.y, abs_dir_v4.z);
				directional_lights.emplace_back(color, attenuation, abs_dir);
				break;
			}
			case aiLightSource_POINT: {
				glm::vec3 rel_pos = to_vec3(ai_light->mPosition);
				glm::vec4 rel_pos_v4 = glm::vec4(rel_pos.x, rel_pos.y, rel_pos.z, 1.0);
				glm::vec4 abs_pos_v4 = transform * rel_pos_v4;
				glm::vec3 abs_pos = glm::vec3(abs_pos_v4.x, abs_pos_v4.y, abs_pos_v4.z);
				point_lights.emplace_back(color, attenuation, abs_pos);
				break;
			}
			default: {
				std::cerr << "unsupported light source " << ai_light->mType << std::endl;
			}
			}
		}
	}
}

void Model::render(Shader& shader, const Camera& camera) const {
	for (const Mesh& mesh : this->meshes) {
		mesh.render(shader, camera);
	}
}
