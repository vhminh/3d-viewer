#include "glwrapper/vertexarray.h"

#include <GLES3/gl3.h>

VertexArray::VertexArray() { glGenVertexArrays(1, &this->id); }

VertexArray::VertexArray(VertexArray&& another) {
	this->id = another.id;
	another.id = ID_NONE;
}

VertexArray& VertexArray::operator=(VertexArray&& another) {
	if (this == &another) {
		return *this;
	}
	if (this->id != ID_NONE) {
		glDeleteVertexArrays(1, &this->id);
	}
	this->id = another.id;
	another.id = ID_NONE;
	return *this;
}

VertexArray::~VertexArray() {
	if (this->id != ID_NONE) {
		glDeleteVertexArrays(1, &this->id);
	}
}

void VertexArray::bind() const { glBindVertexArray(this->id); }
