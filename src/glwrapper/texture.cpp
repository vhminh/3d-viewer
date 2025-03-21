#include "glwrapper/texture.h"

#include "gl.h"
#include "gl3.h"
#include "stb_image.h"
#include "utils/error.h"

#include <cassert>
#include <iostream>

Texture::Texture(GLuint id, TextureFormat format) : GLObject(id), format(format) {}

Texture Texture::create(
	const char* path, TextureFormat format, int wrap_s, int wrap_t, int min_filter, int mag_filter
) {
	// load image
	int w, h, n_channels;
	stbi_set_flip_vertically_on_load(false);
	int desired_channel;
	switch (format) {
	case RGB:
	case SRGB:
		desired_channel = STBI_rgb;
		break;
	case RGBA:
	case SRGBA:
		desired_channel = STBI_rgb_alpha;
		break;
	default:
		std::cerr << "invalid texture format " << format << std::endl;
		assert(false);
		break;
	}
	unsigned char* data = stbi_load(path, &w, &h, &n_channels, desired_channel);
	if (!data) {
		throw ImageLoadingException(path);
	}

	Texture texture = Texture::create(data, w, h, format, wrap_s, wrap_t, min_filter, mag_filter);

	stbi_image_free(data);
	return texture;
}

Texture Texture::create(
	unsigned char* data, int w, int h, TextureFormat format, int wrap_s, int wrap_t, int min_filter, int mag_filter
) {
	// texture wrap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);

	GLint gl_internal_format;
	GLenum gl_input_format;
	switch (format) {
	case RGB:
		gl_internal_format = GL_RGB;
		gl_input_format = GL_RGB;
		break;
	case RGBA:
		gl_internal_format = GL_RGBA;
		gl_input_format = GL_RGBA;
		break;
	case SRGB:
		gl_internal_format = GL_SRGB; // convert to linear space
		gl_input_format = GL_RGB;
		break;
	case SRGBA:
		gl_internal_format = GL_SRGB_ALPHA; // convert to linear space
		gl_input_format = GL_RGBA;
		break;
	default:
		std::cerr << "invalid texture format " << format << std::endl;
		assert(false);
		break;
	}

	// create texture
	GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, gl_internal_format, w, h, 0, gl_input_format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	return Texture(id, format);
}

Texture::Texture(Texture&& another) : GLObject(another.id), format(another.format) {
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
	this->format = another.format;
	another.id = ID_NONE;
	return *this;
}

Texture::~Texture() {
	if (this->id != ID_NONE) {
		glDeleteTextures(1, &this->id);
	}
}
