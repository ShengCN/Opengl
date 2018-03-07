#pragma once
#include "GraphicsBase.h"
class GraphicsLight :
	public GraphicsBase
{
public:
	GraphicsLight();
	~GraphicsLight();

	void Draw() override;
	void Reload() override;
	void Draw_Shader_Uniforms() override;

	void BufferManage() override;
	void ReleaseBuffers() override;
	void Init_Buffers() override;

private:
	GLuint vao;
};

