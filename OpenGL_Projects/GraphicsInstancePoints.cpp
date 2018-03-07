#include "GraphicsInstancePoints.h"
#include "Global_Variables.h"
#include "IchenLib/Utilities.h"

const int num_particles = 256 * 256 * 256;
GraphicsInstancePoints::GraphicsInstancePoints()
{
}


GraphicsInstancePoints::~GraphicsInstancePoints()
{
}

void GraphicsInstancePoints::Draw()
{
	glUseProgram(shader_program);
	auto gv = Global_Variables::Instance();
	const GLint pos_loc = 0;
	const GLint vel_loc = 1;
	const GLint age_loc = 2;

	auto P = gv->current_camera->GetP();
	auto V = gv->current_camera->GetV();
	auto M = glm::rotate(Degree2Radian(gv->float_uniforms["angle"]), glm::vec3(0.0, 1.0, 0.0));
	auto PVM = P*V*M;
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "PVM"), 1, false, glm::value_ptr(PVM));
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "V"), 1, false, glm::value_ptr(V));

	glBindVertexArray(vao);
	glDrawArraysInstanced(GL_POINTS, 0, 1, num_particles);
	glBindVertexArray(0);
}

void GraphicsInstancePoints::Draw_Shader_Uniforms()
{
}

void GraphicsInstancePoints::Reload()
{
	if(shader_program!=-1)
	{
		glDeleteShader(shader_program);
	}

	Init_Shaders(m_vs_file, m_fs_file);
	Init_Buffers();
}

void GraphicsInstancePoints::Init_Buffers()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
}

void GraphicsInstancePoints::BufferManage()
{
}

void GraphicsInstancePoints::ReleaseBuffers()
{
}
