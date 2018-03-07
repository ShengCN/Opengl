#pragma once
#include "GraphicsBase.h"
class GraphicsGrids :
	public GraphicsBase
{
public:
	GraphicsGrids();
	~GraphicsGrids();

	void Draw() override;
	void Reload() override;
	void Draw_Shader_Uniforms() override;

	void BufferManage() override;
	void ReleaseBuffers() override;
	void Init_Buffers() override;

private:
	GLuint vao, vbo, ebo;
};

