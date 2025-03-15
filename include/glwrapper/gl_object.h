#pragma once
#include "utils/macro.h"
#include "gl.h"

class GLObject {
public:
	NO_COPY(GLObject);
	MOVE(GLObject);
	GLObject(GLuint id);
	virtual ~GLObject();
	GLuint get_id() const;
	const static GLuint ID_NONE;

protected:
	GLuint id;
};
