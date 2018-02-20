#pragma once
#include "DrawObjects/GraphicsBase.h"
#include "DrawObjects/IBufferManager.h"

class GraphicsPicker:public GraphicsBase, public IBufferManager
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

	GLuint GetPicker() const { return picker_buffer; }
	GLuint GetTexture() const { return texture_buffer; }
private:
	GLuint frame_buffer,texture_buffer, picker_buffer, render_buffer;
	GLuint vao, vbo, ebo; // quad
};
