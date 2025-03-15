#include "glwrapper/texture.h"

#include "gl.h"
#include "gl3.h"
#include "stb_image.h"
#include "utils/error.h"

Texture::Texture(GLuint id, TextureType type) : GLObject(id), type(type) {}

Texture Texture::create(const char* path, TextureType type, int wrap_s, int wrap_t, int min_filter, int mag_filter) {
	// load image
	int w, h, n_channels;
	stbi_set_flip_vertically_on_load(false);
	int desired_channel;
	switch (type) {
	case ALBEDO:
		desired_channel = STBI_rgb_alpha;
		break;
	default:
		desired_channel = STBI_rgb;
		break;
	}
	unsigned char* data = stbi_load(path, &w, &h, &n_channels, desired_channel);
	if (!data) {
		throw ImageLoadingException(path);
	}

	Texture texture = Texture::create(data, w, h, type, wrap_s, wrap_t, min_filter, mag_filter);

	stbi_image_free(data);
	return texture;
}

Texture Texture::create(
	unsigned char* data, int w, int h, TextureType type, int wrap_s, int wrap_t, int min_filter, int mag_filter
) {
	// texture wrap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);

	GLint internal_format;
	GLenum format;
	switch (type) {
	case ALBEDO:
		internal_format = GL_SRGB_ALPHA; // convert to linear space
		format = GL_RGBA;
		break;
	default:
		internal_format = GL_RGB;
		format = GL_RGB;
		break;
	}

	// create texture
	GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	return Texture(id, type);
}

Texture::Texture(Texture&& another) : GLObject(another.id), type(another.type) {
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
	another.id = ID_NONE;
	return *this;
}

Texture::~Texture() {
	if (this->id != ID_NONE) {
		glDeleteTextures(1, &this->id);
	}
}

TextureType Texture::get_type() const {
	return this->type;
}
