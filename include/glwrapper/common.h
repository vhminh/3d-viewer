#pragma once
#include "util/macro.h"

class GLObject {
public:
	NO_COPY(GLObject);
	GLObject();
	virtual ~GLObject();
	unsigned get_id() const;
	const static unsigned ID_NONE;

protected:
	unsigned id;
};
