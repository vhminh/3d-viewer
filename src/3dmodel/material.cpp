#include "3dmodel/material.h"

PBRMaterial::PBRMaterial(PBRColorTex albedo, PBROptTex normals, PBRPropTex metallic, PBRPropTex roughness,
                         PBRPropTex ambient_occlusion, unsigned albedo_uv_channel, unsigned normals_uv_channel,
                         unsigned metallic_uv_channel, unsigned roughness_uv_channel, unsigned ao_uv_channel)
	: albedo(albedo), normals(normals), metallic(metallic), roughness(roughness), ambient_occlusion(ambient_occlusion),
	  albedo_uv_channel(albedo_uv_channel), normals_uv_channel(normals_uv_channel),
	  metallic_uv_channel(metallic_uv_channel), roughness_uv_channel(roughness_uv_channel),
	  ao_uv_channel(ao_uv_channel) {}
