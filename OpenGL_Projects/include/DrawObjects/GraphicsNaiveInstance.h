#pragma once
#include "GraphicsBase.h"

class GraphicsNaiveInstance : public GraphicsBase
{
public:
	GraphicsNaiveInstance();
	~GraphicsNaiveInstance();


	void Draw() override;
	void Draw_Shader_Uniforms() override;
	void Reload() override;
	void Init_Buffers() override;
	void BufferManage() override;
	void ReleaseBuffers() override;

	void setInstanceNumber(int i)  { instance_number = i; }

	int getInstanceNumber() const { return instance_number; }

	GLuint instance_timer;
private:
	int instance_number;
	std::vector<glm::mat4> uniform_dis;
	std::vector<glm::vec4> uniform_color;
};

