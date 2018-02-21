#pragma once
#include "DrawObjects/GraphicsBase.h"

class GraphicsParticleSystem :
	public GraphicsBase
{
public:
	GraphicsParticleSystem();
	~GraphicsParticleSystem();

	void Draw() override;
	void Draw_Shader_Uniforms() override;
	void Reload() override;
	void Init_Buffers() override;
	void BufferManage() override;
	void ReleaseBuffers() override;

private:
	GLuint vao[2], vbo[2], tfo[2]; 
	int Write_Index, Read_Index;
};

