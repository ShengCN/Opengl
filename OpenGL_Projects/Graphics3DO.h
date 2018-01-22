#pragma once
#include "GraphicsBase.h"
class Graphics3DO :
	public GraphicsBase
{
public:
	Graphics3DO();
	~Graphics3DO();

	void Draw() override;
	void Reload() override;
	void Draw_Shader_Uniforms() override;

	void BufferManage() override;
	void ReleaseBuffers() override;
	void Init_Buffers() override;

private:
	GLuint vao;
};

