#pragma once
#include "GraphicsBase.h"
class GraphicsPoints :
	public GraphicsBase
{
public:
	GraphicsPoints();
	~GraphicsPoints();

	void Draw() override;
	void Reload() override;
	void Draw_Shader_Uniforms() override;

	void BufferManage() override;
	void ReleaseBuffers() override;
	void Init_Buffers() override;

private:
	GLuint vao,vbo;
};

