#pragma once

#include "glwrapper/texture.h"

#include <glm/glm.hpp>
#include <memory>
#include <optional>
#include <variant>

typedef std::optional<std::shared_ptr<Texture>> PBROptTex;
typedef std::variant<std::shared_ptr<Texture>, glm::vec3> PBRColorTex;
typedef std::variant<std::shared_ptr<Texture>, float> PBRPropTex;

struct PBRMaterial {
	PBRMaterial(PBRColorTex albedo, PBROptTex normals, PBRPropTex metallic, PBRPropTex roughness,
	            PBRPropTex ambient_occlusion, unsigned albedo_uv_channel, unsigned normals_uv_channel,
	            unsigned metallic_uv_channel, unsigned roughness_uv_channel, unsigned ao_uv_channel);

	PBRColorTex albedo;
	PBROptTex normals;
	PBRPropTex metallic;
	PBRPropTex roughness;
	PBRPropTex ambient_occlusion;
	unsigned albedo_uv_channel;
	unsigned normals_uv_channel;
	unsigned metallic_uv_channel;
	unsigned roughness_uv_channel;
	unsigned ao_uv_channel;
};
