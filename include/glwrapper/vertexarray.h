#pragma once
#include "glwrapper/common.h"
#include "util/macro.h"

class VertexArray : public GLObject {
public:
	NO_COPY(VertexArray);
	VertexArray();
	VertexArray(VertexArray&&);
	virtual ~VertexArray();
	VertexArray& operator=(VertexArray&&);
	void bind() const;
};
