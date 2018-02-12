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

	GLuint GetTexture() const { return texture_buffer; }
private:
	GLuint frame_buffer,texture_buffer, render_buffer,picker_buffer;
	float currentID;
};

