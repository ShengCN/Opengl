#pragma once
#include "DrawObjects/GraphicsBase.h"

class GraphicsPicker:public GraphicsBase
{
public:
	GraphicsPicker();
	~GraphicsPicker();

	void Draw() override;
	void Draw_Shader_Uniforms() override;
	void Reload() override;
	void Init_Buffers() override;
	void BufferManage() override;
	void ReleaseBuffers() override;
private:
	GLuint picker_buffer;
};

