#pragma once
#include "DrawObjects/GraphicsBase.h"
class GraphicsScripts :
	public GraphicsBase
{
public:
	GraphicsScripts();
	~GraphicsScripts();

	void Draw() override;
	void Draw_Shader_Uniforms() override;
	void Reload() override;
	void Init_Buffers() override;
	void BufferManage() override;
	void ReleaseBuffers() override;

	void SaveScreenShot();
	GLuint GetTexture() { return FBO_texture; }
private:
	GLuint FBO, FBO_texture, RBO; // for screenshot
};

