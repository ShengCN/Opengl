#pragma once
#include "GraphicsBase.h"
#include "IBufferManager.h"

class GraphicsShaderToy: public GraphicsBase, public IBufferManager
{
public:
	GraphicsShaderToy();
	~GraphicsShaderToy();


	void Draw() override;
	void Draw_Shader_Uniforms() override;
	void Reload() override;
	void Init_Buffers() override;
	void BufferManage() override;
	void ReleaseBuffers() override;
	void InitCubemap(std::string cubemapFiles);

private:
	GLuint vao,vbo,ebo;
	GLuint m_volume_texture;
	GLuint m_cubemap;
};

