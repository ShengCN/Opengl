#pragma once
#include "GraphicsBase.h"
#include "IBufferManager.h"

class GraphicsFBO :
	public GraphicsBase, public IBufferManager
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
	GLuint qua_vao,	qua_vbo, qua_ebo; // quadra 
	GLuint sur_vao, sur_vbo, sur_ebo; // surface
	GLuint fbo, texture_buffer,render_buffer; // rendering buffers
	GLuint picker_buffer; // pick an object

};

