#pragma once
#include "GraphicsBase.h"
class GraphicsFish :
	public GraphicsBase
{
public:
	GraphicsFish();
	~GraphicsFish();

	void Draw() override;
	void Reload() override;
	void Draw_Shader_Uniforms() override;

	void BufferManage() override;
	void ReleaseBuffers() override;
	void Init_Buffers() override;

private:

	GLuint transform_buffer,color_buffer;
};

