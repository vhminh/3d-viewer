#pragma once
#include "glwrapper/texture.h"

#include <assimp/scene.h>
#include <glm/glm.hpp>

glm::vec3 from_ai_color3(const aiColor3D& color);

glm::vec4 from_ai_color4(const aiColor4D& color);

glm::vec3 from_ai_vec3(const aiVector3D& vec);

glm::mat4 from_ai_mat4(const aiMatrix4x4& mat);

TextureType from_ai_texture_type(const aiTextureType ai_type);

aiTextureType to_ai_texture_type(const TextureType type);
