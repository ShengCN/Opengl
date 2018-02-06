#pragma once
#include "GraphicsBase.h"
class GraphicsSurface :
	public GraphicsBase
{
public:
	GraphicsSurface();
	~GraphicsSurface();

	void Draw() override;
	void Reload() override;
	void Draw_Shader_Uniforms() override;

	void BufferManage() override;
	void ReleaseBuffers() override;
	void Init_Buffers() override;

private:
	GLuint vao, vbo;
	const int ni = 50; //number of points in each direction
	const int nj = 50;

};

