#include "3dmodel/model.h"

#include "3dmodel/utils/assimp_conversions.h"
#include "app/config.h"
#include "assimp/Importer.hpp"
#include "assimp/light.h"
#include "assimp/material.h"
#include "assimp/types.h"
#include "utils/error.h"
#include "utils/path.h"

#include <array>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <iostream>
#include <stack>

// meshes
std::vector<Mesh> load_meshes(ResourceManager&, const std::string& directory, const aiScene*);
Mesh load_mesh(
	ResourceManager&, const std::string& directory, const aiScene*, const aiMesh*, const glm::mat4& transform
);

// materials
PBRMaterial load_material(ResourceManager&, const std::string& directory, const aiScene*, const aiMaterial*);
std::optional<const char*> get_texture_path(const aiScene*, const aiMaterial*, TextureType);
PBRColorTex load_color_texture(
	ResourceManager&, const std::string& directory, const aiScene*, const aiMaterial*, TextureType,
	const char* fallback_matkey, unsigned fallback_matkey_type, unsigned fallback_matkey_idx
);
PBROptTex load_texure_opt(
	ResourceManager&, const std::string& directory, const aiScene*, const aiMaterial*, TextureType
);
PBRPropTex load_float_texture(
	ResourceManager&, const std::string& directory, const aiScene*, const aiMaterial*, TextureType,
	const char* fallback_matkey, unsigned fallback_matkey_type, unsigned fallback_matkey_idx
);
PBRPropTex load_float_texture(
	ResourceManager&, const std::string& directory, const aiScene*, const aiMaterial*, TextureType, float fallback
);

// lights
std::tuple<std::vector<DirectionalLight>, std::vector<PointLight>> load_lights(const aiScene*);

Model::Model(ResourceManager& resource_manager, const char* path) : directory(directory_of(path)) {

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(
		path,
		aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType
	);

	if (!scene) {
		throw ModelLoadingError(path, importer.GetErrorString());
	}

	std::cout << "lights: " << scene->mNumLights << std::endl;
	std::cout << "cameras: " << scene->mNumCameras << std::endl;
	std::cout << "materials: " << scene->mNumMaterials << std::endl;
	std::cout << "meshes: " << scene->mNumMeshes << std::endl;
	std::cout << "animations: " << scene->mNumAnimations << std::endl;

	this->meshes = std::move(load_meshes(resource_manager, directory, scene));

	std::tie(this->directional_lights, this->point_lights) = load_lights(scene);
	assert(this->directional_lights.size() <= MAX_NUM_DIRECTIONAL_LIGHTS);
	assert(this->point_lights.size() <= MAX_NUM_POINT_LIGHTS);
}

Model::Model(Model&& another) : directory(another.directory) {
	meshes = std::move(another.meshes);
	materials = std::move(another.materials);
	directional_lights = std::move(another.directional_lights);
	point_lights = std::move(another.point_lights);
}

const std::vector<DirectionalLight>& Model::get_directional_lights() const {
	return this->directional_lights;
}

const std::vector<PointLight>& Model::get_point_lights() const {
	return this->point_lights;
}

std::vector<Mesh> load_meshes(ResourceManager& resource_manager, const std::string& directory, const aiScene* scene) {
	std::vector<Mesh> meshes;
	std::stack<std::pair<aiNode*, glm::mat4>> stack;
	stack.push(std::make_pair(scene->mRootNode, glm::mat4(1.0)));
	while (stack.size()) {
		auto [node, transform] = stack.top();
		stack.pop();

		glm::mat4 new_transform = transform * from_ai_mat4(node->mTransformation);

		for (unsigned i = 0; i < node->mNumMeshes; ++i) {
			unsigned mesh_idx = node->mMeshes[i];
			meshes.emplace_back(load_mesh(resource_manager, directory, scene, scene->mMeshes[mesh_idx], new_transform));
		}

		for (int i = 0; i < node->mNumChildren; ++i) {
			stack.push(std::make_pair(node->mChildren[i], new_transform));
		}
	}
	return meshes;
}

Mesh load_mesh(
	ResourceManager& resource_manager, const std::string& directory, const aiScene* scene, const aiMesh* mesh,
	const glm::mat4& transform
) {
	std::vector<Vertex> vertices;
	vertices.reserve(mesh->mNumVertices);
	for (int i = 0; i < mesh->mNumVertices; ++i) {
		std::array<glm::vec2, MAX_NUM_UV_CHANNELS> tex_coords;
		for (int j = 0; j < MAX_NUM_UV_CHANNELS; ++j) {
			if (mesh->mTextureCoords[j]) {
				aiVector3D coord = mesh->mTextureCoords[j][i];
				tex_coords[j] = glm::vec2(coord.x, coord.y);
			} else {
				tex_coords[j] = glm::vec2(0.0, 0.0);
			}
		}
		vertices.emplace_back(
			from_ai_vec3(mesh->mVertices[i]),
			glm::normalize(from_ai_vec3(mesh->mTangents[i])),
			glm::normalize(from_ai_vec3(mesh->mBitangents[i])),
			glm::normalize(from_ai_vec3(mesh->mNormals[i])),
			tex_coords
		);
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

	PBRMaterial material = load_material(resource_manager, directory, scene, scene->mMaterials[mesh->mMaterialIndex]);
	return Mesh(transform, std::move(vertices), std::move(indices), std::move(material));
}

std::ostream& operator<<(std::ostream& os, const aiColor3D& color) {
	os << "(" << color.r << "," << color.g << "," << color.b << ")";
	return os;
}

PBRMaterial load_material(
	ResourceManager& resource_manager, const std::string& directory, const aiScene* scene, const aiMaterial* material
) {
	PBRColorTex albedo =
		load_color_texture(resource_manager, directory, scene, material, TextureType::ALBEDO, AI_MATKEY_BASE_COLOR);
	PBROptTex normals = load_texure_opt(resource_manager, directory, scene, material, TextureType::NORMALS);
	PBRPropTex metallic = load_float_texture(
		resource_manager, directory, scene, material, TextureType::METALLIC, AI_MATKEY_METALLIC_FACTOR
	);
	PBRPropTex roughness = load_float_texture(
		resource_manager, directory, scene, material, TextureType::ROUGHNESS, AI_MATKEY_ROUGHNESS_FACTOR
	);
	PBRPropTex ambient_occlusion =
		load_float_texture(resource_manager, directory, scene, material, TextureType::AMBIENT_OCCLUSION, 1.0);
	unsigned albedo_uv_channel = 0;
	material->Get(AI_MATKEY_UVWSRC(aiTextureType_BASE_COLOR, 0), albedo_uv_channel);
	unsigned normals_uv_channel = 0;
	material->Get(AI_MATKEY_UVWSRC(aiTextureType_NORMALS, 0), normals_uv_channel);
	unsigned metallic_uv_channel = 0;
	material->Get(AI_MATKEY_UVWSRC(aiTextureType_METALNESS, 0), metallic_uv_channel);
	unsigned roughness_uv_channel = 0;
	material->Get(AI_MATKEY_UVWSRC(aiTextureType_DIFFUSE_ROUGHNESS, 0), roughness_uv_channel);
	unsigned ao_uv_channel = 0;
	material->Get(AI_MATKEY_UVWSRC(aiTextureType_AMBIENT_OCCLUSION, 0), ao_uv_channel);

	return PBRMaterial(
		albedo,
		normals,
		metallic,
		roughness,
		ambient_occlusion,
		albedo_uv_channel,
		normals_uv_channel,
		metallic_uv_channel,
		roughness_uv_channel,
		ao_uv_channel
	);
}

std::optional<const char*> get_texture_path(
	const aiScene* scene, const aiMaterial* material, TextureType texture_type
) {
	aiTextureType ai_type = to_ai_texture_type(texture_type);
	std::cerr << "Material " << material->GetName().C_Str() << ", texture type: " << texture_type
			  << ", count: " << material->GetTextureCount(ai_type) << std::endl;
	if (material->GetTextureCount(ai_type) == 0) {
		return std::nullopt;
	} else {
		aiString rel_path;                           // path relative to main model file
		material->GetTexture(ai_type, 0, &rel_path); // TODO: multi texture support
		std::cerr << "path is " << rel_path.C_Str() << std::endl;
		return std::make_optional(rel_path.C_Str());
	}
}

PBROptTex load_texure_opt(
	ResourceManager& resource_manager, const std::string& directory, const aiScene* scene, const aiMaterial* material,
	TextureType type
) {
	std::optional<const char*> maybe_path = get_texture_path(scene, material, type);
	if (maybe_path) {
		// has texture
		return resource_manager.load_texture(scene, type, directory.c_str(), maybe_path.value());
	} else {
		return std::nullopt;
	}
}

PBRColorTex load_color_texture(
	ResourceManager& resource_manager, const std::string& directory, const aiScene* scene, const aiMaterial* material,
	TextureType type, const char* fallback_matkey, unsigned fallback_matkey_type, unsigned fallback_matkey_idx
) {
	std::optional<const char*> maybe_path = get_texture_path(scene, material, type);
	if (maybe_path) {
		// has texture
		return resource_manager.load_texture(scene, type, directory.c_str(), maybe_path.value());
	} else {
		// fallback color
		aiColor3D color;
		assert(aiReturn_SUCCESS == material->Get(fallback_matkey, fallback_matkey_type, fallback_matkey_idx, color));
		return from_ai_color3(color);
	}
}

PBRPropTex load_float_texture(
	ResourceManager& resource_manager, const std::string& directory, const aiScene* scene, const aiMaterial* material,
	TextureType type, const char* fallback_matkey, unsigned fallback_matkey_type, unsigned fallback_matkey_idx
) {
	std::optional<const char*> maybe_path = get_texture_path(scene, material, type);
	if (maybe_path) {
		// has texture
		return resource_manager.load_texture(scene, type, directory.c_str(), maybe_path.value());
	} else {
		// fallback value
		float value;
		assert(aiReturn_SUCCESS == material->Get(fallback_matkey, fallback_matkey_type, fallback_matkey_idx, value));
		return value;
	}
}

PBRPropTex load_float_texture(
	ResourceManager& resource_manager, const std::string& directory, const aiScene* scene, const aiMaterial* material,
	TextureType type, float fallback
) {
	std::optional<const char*> maybe_path = get_texture_path(scene, material, type);
	if (maybe_path) {
		// has texture
		return resource_manager.load_texture(scene, type, directory.c_str(), maybe_path.value());
	} else {
		// fallback value
		return fallback;
	}
}

void transforms_by_node_name_helper(
	const aiNode* node, const glm::mat4& prev_transform, std::map<std::string_view, std::vector<glm::mat4>>& result
) {
	std::string_view node_name = std::string_view(node->mName.C_Str());
	glm::mat4 transform = prev_transform * from_ai_mat4(node->mTransformation);
	if (node_name.size()) {
		result[node_name].push_back(prev_transform * from_ai_mat4(node->mTransformation));
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

std::tuple<std::vector<DirectionalLight>, std::vector<PointLight>> load_lights(const aiScene* scene) {
	std::vector<DirectionalLight> directional_lights;
	std::vector<PointLight> point_lights;

	std::map<std::string_view, std::vector<glm::mat4>> transforms_by_name = transforms_by_node_name(scene);
	for (int i = 0; i < scene->mNumLights; ++i) {
		aiLight* ai_light = scene->mLights[i];
		glm::vec3 color = from_ai_color3(ai_light->mColorDiffuse);
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
				glm::vec3 rel_dir = from_ai_vec3(ai_light->mDirection);
				glm::vec4 rel_dir_v4 = glm::vec4(rel_dir.x, rel_dir.y, rel_dir.z, 0.0);
				glm::vec4 abs_dir_v4 = transform * rel_dir_v4;
				glm::vec3 abs_dir = glm::vec3(abs_dir_v4.x, abs_dir_v4.y, abs_dir_v4.z);
				directional_lights.emplace_back(color, abs_dir);
				break;
			}
			case aiLightSource_POINT: {
				glm::vec3 rel_pos = from_ai_vec3(ai_light->mPosition);
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
	return std::make_tuple(directional_lights, point_lights);
}

void Model::render(Shader& shader) const {
	for (const Mesh& mesh : this->meshes) {
		mesh.render(shader);
	}
}
