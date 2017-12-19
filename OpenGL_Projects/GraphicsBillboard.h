#pragma once
#include "GraphicsBase.h"
#include "IchenLib/LoadTexture.h"

class GraphicsBillboard :
	public GraphicsBase
{
public:
	GraphicsBillboard();
	~GraphicsBillboard();

	void Draw() override;
	void Reload() override;
	void Draw_Shader_Uniforms() override;

	void BufferManage() override;
	void ReleaseBuffers() override;
	void Init_Buffers() override;

private:
	GLuint m_VAO;
	GLuint m_VBillboard;
};

