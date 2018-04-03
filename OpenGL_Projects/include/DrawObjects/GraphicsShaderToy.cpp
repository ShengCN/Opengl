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
	int V_loc = glGetUniformLocation(shader_program, "volume_tex");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, volume_tex);
	glUniform1i(V_loc, 0);
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
	volume_tex = LoadTexture3D(gv->volume_dir + gv->volume_data);
}

void GraphicsShaderToy::BufferManage()
{
}

void GraphicsShaderToy::ReleaseBuffers()
{
}
