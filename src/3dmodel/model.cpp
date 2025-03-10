#include "3dmodel/model.h"

#include "3dmodel/mesh.h"
#include "3dmodel/utils/ai_conversions.h"
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
#include <iostream>
#include <stack>
#include <vector>

Vertex create_vertex(const aiVector3D& position, const aiVector3D& tangent, const aiVector3D& bitangent,
                     const aiVector3D& normal, const std::array<glm::vec2, MAX_NUM_UV_CHANNELS> tex_coords);

std::string directory_of(const char* path) {
	std::string directory = std::string(path);
	directory.erase(directory.find_last_of('/'));
	return directory;
}

Model::Model(const char* path) : directory(directory_of(path)) {
	const aiScene* scene =
		importer.ReadFile(path, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_FlipUVs |
	                                aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);

	if (!scene) {
		std::cerr << "cannot load scene: " << importer.GetErrorString() << std::endl;
	}

	load_lights(scene);

	load_meshes(scene);

	std::cout << "lights: " << scene->mNumLights << std::endl;
	std::cout << "cameras: " << scene->mNumCameras << std::endl;
	std::cout << "materials: " << scene->mNumMaterials << std::endl;
	std::cout << "meshes: " << scene->mNumMeshes << std::endl;
	std::cout << "animations: " << scene->mNumAnimations << std::endl;
}

void Model::load_meshes(const aiScene* scene) {
	std::stack<std::pair<aiNode*, glm::mat4>> stack;
	stack.push(std::make_pair(scene->mRootNode, glm::mat4(1.0)));
	while (stack.size()) {
		auto [node, transform] = stack.top();
		stack.pop();

		glm::mat4 new_transform = transform * from_ai_mat4(node->mTransformation);

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
			create_vertex(mesh->mVertices[i], mesh->mTangents[i], mesh->mBitangents[i], mesh->mNormals[i], tex_coords));
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

	PBRMaterial material = load_material(scene, scene->mMaterials[mesh->mMaterialIndex]);
	return Mesh(transform, std::move(vertices), std::move(indices), std::move(material));
}

Vertex create_vertex(const aiVector3D& position, const aiVector3D& tangent, const aiVector3D& bitangent,
                     const aiVector3D& normal, const std::array<glm::vec2, MAX_NUM_UV_CHANNELS> tex_coords) {
	return Vertex(from_ai_vec3(position), glm::normalize(from_ai_vec3(tangent)),
	              glm::normalize(from_ai_vec3(bitangent)), glm::normalize(from_ai_vec3(normal)), tex_coords);
}

std::ostream& operator<<(std::ostream& os, const aiColor3D& color) {
	os << "(" << color.r << "," << color.g << "," << color.b << ")";
	return os;
}

PBRMaterial Model::load_material(const aiScene* scene, const aiMaterial* material) {
	PBRColorTex albedo = load_color_texture(scene, material, TextureType::ALBEDO, AI_MATKEY_BASE_COLOR);
	PBROptTex normals = load_texure_opt(scene, material, TextureType::NORMALS);
	PBRPropTex metallic = load_float_texture(scene, material, TextureType::METALLIC, AI_MATKEY_METALLIC_FACTOR);
	PBRPropTex roughness = load_float_texture(scene, material, TextureType::ROUGHNESS, AI_MATKEY_ROUGHNESS_FACTOR);
	PBRPropTex ambient_occlusion = load_float_texture(scene, material, TextureType::AMBIENT_OCCLUSION, 1.0);
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

	return PBRMaterial(albedo, normals, metallic, roughness, ambient_occlusion, albedo_uv_channel, normals_uv_channel,
	                   metallic_uv_channel, roughness_uv_channel, ao_uv_channel);
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

std::optional<const char*> get_texture_path(const aiScene* scene, const aiMaterial* material,
                                            TextureType texture_type) {
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

PBROptTex Model::load_texure_opt(const aiScene* scene, const aiMaterial* material, TextureType type) {
	std::optional<const char*> maybe_path = get_texture_path(scene, material, type);
	if (maybe_path) {
		// has texture
		return load_texture(scene, type, maybe_path.value());
	} else {
		return std::nullopt;
	}
}

PBRColorTex Model::load_color_texture(const aiScene* scene, const aiMaterial* material, TextureType type,
                                      const char* fallback_matkey, unsigned fallback_matkey_type,
                                      unsigned fallback_matkey_idx) {
	std::optional<const char*> maybe_path = get_texture_path(scene, material, type);
	if (maybe_path) {
		// has texture
		return load_texture(scene, type, maybe_path.value());
	} else {
		// fallback color
		aiColor3D color;
		assert(aiReturn_SUCCESS == material->Get(fallback_matkey, fallback_matkey_type, fallback_matkey_idx, color));
		return from_ai_color3(color);
	}
}

PBRPropTex Model::load_float_texture(const aiScene* scene, const aiMaterial* material, TextureType type,
                                     const char* fallback_matkey, unsigned fallback_matkey_type,
                                     unsigned fallback_matkey_idx) {
	std::optional<const char*> maybe_path = get_texture_path(scene, material, type);
	if (maybe_path) {
		// has texture
		return load_texture(scene, type, maybe_path.value());
	} else {
		// fallback value
		float value;
		assert(aiReturn_SUCCESS == material->Get(fallback_matkey, fallback_matkey_type, fallback_matkey_idx, value));
		return value;
	}
}

PBRPropTex Model::load_float_texture(const aiScene* scene, const aiMaterial* material, TextureType type,
                                     float fallback) {
	std::optional<const char*> maybe_path = get_texture_path(scene, material, type);
	if (maybe_path) {
		// has texture
		return load_texture(scene, type, maybe_path.value());
	} else {
		// fallback value
		return fallback;
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

void Model::load_lights(const aiScene* scene) {
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
	assert(directional_lights.size() <= MAX_NUM_DIRECTIONAL_LIGHTS);
	assert(point_lights.size() <= MAX_NUM_POINT_LIGHTS);
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

void Model::render(Shader& shader, const Camera& camera) const {
	shader.use();

	// TODO: make this a feature, not a hack lol
	glm::vec3 light_color = glm::vec3(0.2, 0.2, 0.2);
	PointLight player_light = PointLight(light_color, Attenuation(1.0, 0.14, 0.07),
	                                     glm::vec3(camera.origin.x, camera.origin.y + 1.0, camera.origin.z));
	std::vector<PointLight> point_lights = this->point_lights;
	point_lights.push_back(player_light);

	set_light_uniforms(shader, directional_lights, point_lights);
	set_camera_view_transforms(shader, camera);
	for (const Mesh& mesh : this->meshes) {
		mesh.render(shader, camera, directional_lights, point_lights);
	}
}
