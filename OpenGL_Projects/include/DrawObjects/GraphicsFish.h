#pragma once
#include "GraphicsBase.h"
class GraphicsFish :
	public GraphicsBase
{
public:
	GraphicsFish();
	~GraphicsFish();

	void Draw() override;
	void Reload() override;
	void Draw_Shader_Uniforms() override;

	void BufferManage() override;
	void ReleaseBuffers() override;
	void Init_Buffers() override;

	void setInstanceNumber(int i) { instance_number = i; }

	int getInstanceNumber() const { return instance_number;}

private:
	GLuint transform_buffer,color_buffer;
	GLuint instance_timer;
	unsigned int instance_number;
	std::vector<glm::mat4> uniform_dis;
	std::vector<glm::vec4> uniform_color;
};

