#pragma once
#include "GL/glew.h"

// Interface of init buffers
class IBufferManager
{
public:
	void InitQuad(GLuint& vao, GLuint& vbo, GLuint& ebo);
};

