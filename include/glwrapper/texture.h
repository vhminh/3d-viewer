#pragma once
#include "glwrapper/gl_object.h"
#include "util/macro.h"

enum TextureType {
	NORMALS = 0,
	AMBIENT,
	DIFFUSE,
	SPECULAR,

	COUNT,
};

class Texture : public GLObject {
public:
	NO_COPY(Texture);
	MOVE(Texture);
	static Texture create(const char* path, TextureType type, int wrap_s = GL_REPEAT, int wrap_t = GL_REPEAT,
	                      int min_filter = GL_LINEAR, int mag_filter = GL_LINEAR);
	virtual ~Texture();

	TextureType get_type() const;

private:
	Texture(GLuint id, TextureType type);
	TextureType type;
};
