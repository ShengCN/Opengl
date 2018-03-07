#pragma once
#include "DrawObjects\GraphicsBase.h"
class GraphicsInstancePoints :
	public GraphicsBase
{
public:
	GraphicsInstancePoints();
	~GraphicsInstancePoints();

	void Draw() override;
	void Draw_Shader_Uniforms() override;
	void Reload() override;
	void Init_Buffers() override;
	void BufferManage() override;
	void ReleaseBuffers() override;

private:
	GLuint vao, vbo;
};

