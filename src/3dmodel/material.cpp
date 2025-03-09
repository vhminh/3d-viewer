#include "3dmodel/material.h"

PBRMaterial::PBRMaterial(PBRColorTex albedo, PBROptTex normals, PBRPropTex metallic, PBRPropTex roughness,
                         PBRPropTex ambient_occlusion)
	: albedo(albedo), normals(normals), metallic(metallic), roughness(roughness), ambient_occlusion(ambient_occlusion) {}
