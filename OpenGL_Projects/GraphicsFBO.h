#pragma once
#include "GraphicsBase.h"
class GraphicsFBO :
	public GraphicsBase
{
public:
	GraphicsFBO();
	~GraphicsFBO();

	void Draw() override;
	void Draw_Shader_Uniforms() override;
	void Reload() override;
	void Init_Buffers() override;
	void BufferManage() override;
	void ReleaseBuffers() override;
	
private:
	GLuint fbo, texture_buffer,render_buffer;
};

