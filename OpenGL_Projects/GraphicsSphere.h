#pragma once
#include "GraphicsBase.h"
class GraphicsSphere :
	public GraphicsBase
{
public:
	GraphicsSphere();
	~GraphicsSphere();

	void Draw(glm::mat4 P, glm::mat4 V, glm::mat4 M) override;
	virtual void Reload() override;
	virtual void Draw_Shader_Uniforms() override;

	virtual void BufferManage() override;
	virtual void ReleaseBuffers() override;
	virtual void Init_Buffers() override;

private:
	GLuint vao;
};

