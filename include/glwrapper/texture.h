#pragma once
#include "glwrapper/gl_object.h"
#include "utils/macro.h"

enum TextureType {
	BASE_COLOR = 0,
	NORMALS,
	METALLIC,
	ROUGHNESS,
	AMBIENT_OCCLUSION,

	COUNT,
};

enum TextureFormat {
	RGB = 1,
	SRGB,
	RGBA,
	SRGBA,
};

class Texture : public GLObject {
public:
	NO_COPY(Texture);
	MOVE(Texture);
	static Texture create(
		const char* path, TextureFormat format, int wrap_s = GL_REPEAT, int wrap_t = GL_REPEAT,
		int min_filter = GL_LINEAR, int mag_filter = GL_LINEAR
	);
	static Texture create(
		unsigned char* data, int w, int h, TextureFormat format, int wrap_s = GL_REPEAT, int wrap_t = GL_REPEAT,
		int min_filter = GL_LINEAR, int mag_filter = GL_LINEAR
	);
	virtual ~Texture();

private:
	Texture(GLuint id, TextureFormat type);
	TextureFormat format;
};
