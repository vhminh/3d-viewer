#include "glwrapper/texture.h"

#include "gl.h"
#include "gl3.h"
#include "stb_image.h"
#include "util/error.h"

Texture::Texture(GLuint id, const char* path, TextureType type) : GLObject(id), path(path), type(type) {}

Texture Texture::create(const char* path, TextureType type, int wrap_s, int wrap_t, int min_filter, int mag_filter) {
	// texture wrap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);

	// load image
	int w, h, n_channels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path, &w, &h, &n_channels, 0);
	if (!data) {
		throw ImageLoadingException(path);
	}

	// create texture
	GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	return Texture(id, path, type);
}

Texture::Texture(Texture&& another) : GLObject(another.id), path(std::move(another.path)), type(another.type) {
	another.id = ID_NONE;
}

Texture& Texture::operator=(Texture&& another) {
	if (this == &another) {
		return *this;
	}
	if (this->id != ID_NONE) {
		glDeleteTextures(1, &this->id);
	}
	this->id = another.id;
	this->type = another.type;
	this->path = std::move(another.path);
	another.id = ID_NONE;
	return *this;
}

Texture::~Texture() {
	if (this->id != ID_NONE) {
		glDeleteTextures(1, &this->id);
	}
}

const std::string& Texture::get_path() const { return this->path; }

TextureType Texture::get_type() const { return this->type; }
