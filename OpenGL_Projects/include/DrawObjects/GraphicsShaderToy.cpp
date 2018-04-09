#include "GraphicsShaderToy.h"
#include "GLCommon.h"
#include "IchenLib/LoadTexture.h"

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
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glUniform1i(glGetUniformLocation(shader_program, "volume_tex"), 0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
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
	auto gv = Global_Variables::Instance();
	glUseProgram(shader_program);
	InitQuad(vao, vbo, ebo);
}

void GraphicsShaderToy::BufferManage()
{
}

void GraphicsShaderToy::ReleaseBuffers()
{
}

void GraphicsShaderToy::SetTexture(const GLuint texture_id)
{
	m_texture = texture_id;
}

