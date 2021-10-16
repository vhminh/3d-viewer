#include "glwrapper/common.h"

const unsigned GLObject::ID_NONE = -1;

GLObject::GLObject() : id(GLObject::ID_NONE) {}

unsigned GLObject::get_id() const { return id; }

GLObject::~GLObject() {}
