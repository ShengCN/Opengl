#pragma once
#include "DrawObjects/GraphicsBase.h"
#include "DrawObjects/IBufferManager.h"

class GraphicsVolumeRendering :
	public GraphicsBase, public IBufferManager
{
public:
	GraphicsVolumeRendering();
	~GraphicsVolumeRendering();

	void Draw() override;
	void Draw_Shader_Uniforms() override;
	void Reload() override;
	void Init_Buffers() override;
	void BufferManage() override;
	void ReleaseBuffers() override;
private:
	GLuint vao, vbo;
	GLuint fbo,fbo_texture;
	GLuint volume_texture;  // 3D texture
};

