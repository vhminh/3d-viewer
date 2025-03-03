#pragma once
#include "glwrapper/gl_object.h"
#include "util/macro.h"

#include <string>

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

	const std::string& get_path() const;
	TextureType get_type() const;

private:
	Texture(GLuint id, const char* path, TextureType type);
	std::string path;
	TextureType type;
};
