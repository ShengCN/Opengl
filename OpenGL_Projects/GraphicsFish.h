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

private:
<<<<<<< HEAD
	GLuint vbo;
	std::vector<glm::vec3> displacement;
};

=======
	GLuint vao,color_vbo, position_vbo;
};
>>>>>>> Instance
