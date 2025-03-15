#include "app/ResourceManager.h"

#include <iostream>

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

std::shared_ptr<Texture> ResourceManager::load_texture(const aiScene* scene, TextureType type, const char* directory,
                                                       const char* relative_path) {
	std::string path = std::string(directory) + "/" + relative_path;
	return this->load_texture(scene, type, path.c_str());
}

std::shared_ptr<Texture> ResourceManager::load_texture(const aiScene* scene, TextureType type, const char* path) {
	if (texture_by_path.contains(path)) {
		return texture_by_path.at(path);
	}
	std::cerr << "load texture " << type << " " << path << std::endl;
	const aiTexture* embedded_texture = scene->GetEmbeddedTexture(path);
	if (embedded_texture) {
		auto [data, w, h] = texture_data(embedded_texture);
		std::shared_ptr<Texture> ptr = std::make_shared<Texture>(Texture::create(data.data(), w, h, type));
		this->textures.push_back(ptr);
		texture_by_path[path] = ptr;
		return ptr;
	} else {
		std::shared_ptr<Texture> ptr = std::make_shared<Texture>(Texture::create(path, type));
		this->textures.push_back(ptr);
		texture_by_path[path] = ptr;
		return ptr;
	}
}
