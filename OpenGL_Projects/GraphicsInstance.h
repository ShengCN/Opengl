#pragma once
#include "GraphicsBase.h"

class GraphicsInstance :
	public GraphicsBase
{
public:
	GraphicsInstance();
	~GraphicsInstance();

	void Draw() override;
	void Reload() override;
	void Draw_Shader_Uniforms() override;

	void BufferManage() override;
	void ReleaseBuffers() override;
	void Init_Buffers() override;
private:
	GLuint vao,vbo,offset_vbo;
};

