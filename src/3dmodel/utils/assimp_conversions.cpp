#include "3dmodel/utils/assimp_conversions.h"

#include <iostream>

glm::vec3 from_ai_color3(const aiColor3D& color) { return glm::vec3(color.r, color.g, color.b); }

glm::vec3 from_ai_vec3(const aiVector3D& vec) { return glm::vec3(vec.x, vec.y, vec.z); }

glm::mat4 from_ai_mat4(const aiMatrix4x4& mat) {
	// clang-format off
	return glm::mat4(
		mat.a1, mat.b1, mat.c1, mat.d1,
		mat.a2, mat.b2, mat.c2, mat.d2,
		mat.a3, mat.b3, mat.c3, mat.d3,
		mat.a4, mat.b4, mat.c4, mat.d4
	);
	// clang-format on
}

TextureType from_ai_texture_type(const aiTextureType ai_type) {
	if (ai_type == aiTextureType_BASE_COLOR) {
		return TextureType::ALBEDO;
	} else if (ai_type == aiTextureType_NORMALS) {
		return TextureType::NORMALS;
	} else if (ai_type == aiTextureType_METALNESS) {
		return TextureType::METALLIC;
	} else if (ai_type == aiTextureType_DIFFUSE_ROUGHNESS) {
		return TextureType::ROUGHNESS;
	} else if (ai_type == aiTextureType_AMBIENT_OCCLUSION) {
		return TextureType::AMBIENT_OCCLUSION;
	}
	std::cerr << "unsupported assimp texture type " << ai_type << std::endl;
	assert(false);
}

aiTextureType to_ai_texture_type(const TextureType type) {
	switch (type) {
	case TextureType::ALBEDO:
		return aiTextureType_BASE_COLOR;
	case TextureType::NORMALS:
		return aiTextureType_NORMALS;
	case TextureType::METALLIC:
		return aiTextureType_METALNESS;
	case TextureType::ROUGHNESS:
		return aiTextureType_DIFFUSE_ROUGHNESS;
	case TextureType::AMBIENT_OCCLUSION:
		return aiTextureType_AMBIENT_OCCLUSION;
	case TextureType::COUNT:
		assert(false);
	}
	std::cerr << "unsupported texture type " << type << std::endl;
	assert(false);
}
