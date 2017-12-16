#pragma once
#include "GraphicsBase.h"
class GraphicsSphere :
	public GraphicsBase
{
public:
	GraphicsSphere();
	~GraphicsSphere();

	virtual void Draw(glm::mat4 P, glm::mat4 V, glm::mat4 M);
	virtual void Reload();
	virtual void Draw_Shader_Uniforms() = 0;

	virtual void BufferManage();
	virtual void ReleaseBuffers();
	virtual void Init_Buffers();

private:
	GLuint vao;
};

