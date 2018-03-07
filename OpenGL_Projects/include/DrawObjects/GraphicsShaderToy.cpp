#include "GraphicsShaderToy.h"
#include "GLCommon.h"

GraphicsShaderToy::GraphicsShaderToy()
{
}


GraphicsShaderToy::~GraphicsShaderToy()
{
}

void GraphicsShaderToy::Draw()
{
	glUseProgram(shader_program);
	glBindVertexArray(vao);
	// mode indice_number 
	glDisable(GL_DEPTH_TEST);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);
}

void GraphicsShaderToy::Draw_Shader_Uniforms()
{
}

void GraphicsShaderToy::Reload()
{
	if (shader_program != -1)
	{
		glDeleteProgram(shader_program);
	}

	Init_Shaders(m_vs_file, m_fs_file);
	Init_Buffers();
}

void GraphicsShaderToy::Init_Buffers()
{
	glUseProgram(shader_program);
	InitQuad(vao, vbo, ebo);
}

void GraphicsShaderToy::BufferManage()
{
}

void GraphicsShaderToy::ReleaseBuffers()
{
}
