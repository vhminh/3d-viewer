#pragma once
#include <string>
#include <GLES3/gl3.h>
#include "common.h"

class Texture : public GLObject {
public:
    Texture();
    Texture(const std::string& path, int wrap_s = GL_REPEAT, int wrap_t = GL_REPEAT, int min_filter = GL_LINEAR, int mag_filter = GL_LINEAR);
    Texture(Texture const&) = delete;
    Texture& operator=(Texture const&) = delete;
    void move_from(Texture& another);
    ~Texture();
};