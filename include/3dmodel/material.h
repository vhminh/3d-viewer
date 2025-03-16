#pragma once

#include "glwrapper/texture.h"

#include <glm/glm.hpp>
#include <memory>
#include <optional>

typedef std::optional<std::shared_ptr<Texture>> MaybeTexture;

struct PBRMaterial {
	// USE AGGREGATE INIT PLEASE

	const std::optional<std::shared_ptr<Texture>> base_color;
	const glm::vec4 base_color_factor;
	const float opacity;
	const std::optional<std::shared_ptr<Texture>> normals;
	const std::optional<std::shared_ptr<Texture>> metallic;
	const float metallic_factor;
	const std::optional<std::shared_ptr<Texture>> roughness;
	const float roughness_factor;
	const std::optional<std::shared_ptr<Texture>> ambient_occlusion;
	const float ao_strength;

	const unsigned base_color_uv_channel;
	const unsigned normals_uv_channel;
	const unsigned metallic_uv_channel;
	const unsigned roughness_uv_channel;
	const unsigned ao_uv_channel;
};
