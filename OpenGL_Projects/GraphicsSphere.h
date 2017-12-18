#pragma once
#include "GraphicsBase.h"
class GraphicsSphere :
	public GraphicsBase
{
public:
	GraphicsSphere();
	~GraphicsSphere();

<<<<<<< HEAD
	void Draw() override;
	void Reload() override;
	void Draw_Shader_Uniforms() override;

	void BufferManage() override;
	void ReleaseBuffers() override;
	void Init_Buffers() override;
=======
	void Draw(glm::mat4 P, glm::mat4 V, glm::mat4 M) override;
	virtual void Reload() override;
	virtual void Draw_Shader_Uniforms() override;

	virtual void BufferManage() override;
	virtual void ReleaseBuffers() override;
	virtual void Init_Buffers() override;
>>>>>>> 6d2ea0ea3de1622d3487dfc54902fff3a9e5d880

private:
	GLuint vao;
};

