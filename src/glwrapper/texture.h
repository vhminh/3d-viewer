#pragma once
#include <string>
#include <GLES3/gl3.h>
#include "common.h"

class Texture : public GLObject {
public:
    Texture(const std::string& path, int wrap_s = GL_REPEAT, int wrap_t = GL_REPEAT, int min_filter = GL_LINEAR, int mag_filter = GL_LINEAR);
    ~Texture();
};