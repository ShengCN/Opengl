#pragma once
#include "GraphicsBase.h"

class GraphicsShaderToy: public GraphicsBase
{
public:
	GraphicsShaderToy();
	~GraphicsShaderToy();


	void Draw() override;
	void Draw_Shader_Uniforms() override;
	void Reload() override;
	void Init_Buffers() override;
	void BufferManage() override;
	void ReleaseBuffers() override;

private:
	GLuint vao,vbo,ebo;
};

