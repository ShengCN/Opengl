#pragma once
#include "GL/glew.h"
#include <glm/gtc/type_ptr.hpp>

// Interface of init buffers
class IBufferManager
{
public:
	void InitQuad(GLuint& vao, GLuint& vbo, GLuint& ebo);
	void InitSurface(GLuint& vao, GLuint& vbo, GLuint& ebo);

	void DrawSurfaceVao(GLuint vao);
private:
	glm::vec3 normal(float x, float y);
	glm::vec3 surface(float x, float y);
};

