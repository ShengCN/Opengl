#pragma once
#include "GraphicsBase.h"
class GraphicsInstances :
	public GraphicsBase
{
public:
	GraphicsInstances();
	~GraphicsInstances();

	void Draw() override;
	void Draw_Shader_Uniforms() override;
	void Reload() override;
	void Init_Buffers() override;
	void BufferManage() override;
	void ReleaseBuffers() override;
private:
	GLuint vao,vbo,instance_buffer,ebo;
};

