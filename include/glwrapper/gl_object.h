#pragma once
#include "gl.h"
#include "utils/macro.h"

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
