#include "glwrapper/gl_object.h"

const GLuint GLObject::ID_NONE = -1;

GLObject::GLObject(GLuint id) : id(id) {}

GLuint GLObject::get_id() const { return id; }

GLObject::~GLObject() {}
