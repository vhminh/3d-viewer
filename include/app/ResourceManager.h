#pragma once

#include "assimp/scene.h"
#include "glwrapper/texture.h"

#include <map>
#include <memory>
#include <vector>
class ResourceManager {
public:
	ResourceManager() = default;
	NO_COPY(ResourceManager);
	NO_MOVE(ResourceManager);

	std::shared_ptr<Texture> load_texture(
		const aiScene* scene, TextureFormat format, const char* directory, const char* relative_path
	);
	std::shared_ptr<Texture> load_texture(const aiScene* scene, TextureFormat format, const char* path);

private:
	std::vector<std::shared_ptr<Texture>> textures; // TODO: can hold `Texture`s in place
	std::map<std::tuple<std::string, TextureFormat>, std::shared_ptr<Texture>> texture_by_path;
};
