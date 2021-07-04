#include "VertexArray.h"
#include <GLES3/gl3.h>

VertexArray::VertexArray() {
    glGenVertexArrays(1, &this->id);
}

void VertexArray::bind() const {
    glBindVertexArray(this->id);
}

