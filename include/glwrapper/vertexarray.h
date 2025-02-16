#pragma once
#include "glwrapper/common.h"
#include "util/macro.h"

class VertexArray : public GLObject {
public:
	NO_COPY(VertexArray);
	MOVE(VertexArray);
	VertexArray();
	virtual ~VertexArray();
	void bind() const;
};
