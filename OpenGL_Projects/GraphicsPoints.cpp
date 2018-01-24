#include "GraphicsPoints.h"
#include "IchenLib/OpenGLHelpers.h"
#include "Global_Variables.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Common.h"

GraphicsPoints::GraphicsPoints()
{
}


GraphicsPoints::~GraphicsPoints()
{
}

void GraphicsPoints::Draw()
{
	glUseProgram(shader_program);
	auto gv = Global_Variables::Instance();

	// PVM
	glm::mat4 M = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));
	//glm::mat4 M = T * glm::scale(glm::vec3(m_mesh.mScaleFactor*0.1f));
	M = glm::rotate(float_uniforms["angle"], glm::vec3(0.0f, 1.0f, 0.0f)) * M;
	glm::mat4 V = gv->current_camera->GetV();
	glm::mat4 P = gv->current_camera->GetP();

	// variables
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "PVM"), 1, false, glm::value_ptr(P*V*M));
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "P"), 1, false, glm::value_ptr(P));

	glBindVertexArray(vao);
	glDrawArrays(GL_POINTS, 0, 100);

	glBindVertexArray(0);
}

void GraphicsPoints::Reload()
{
	if (shader_program != -1)
	{
		glDeleteProgram(shader_program);
	}

	Init_Shaders(m_vs_file, m_fs_file);
}

void GraphicsPoints::Draw_Shader_Uniforms()
{
}

void GraphicsPoints::BufferManage()
{
}

void GraphicsPoints::ReleaseBuffers()
{
}

void GraphicsPoints::Init_Buffers()
{
	// set up particles initial positions
	std::vector<glm::vec3> positions(100);
	unsigned int index = 0;
	for(auto i = 0; i < 10; ++i)
	{
		for(auto j = 0; j < 10; ++j)
		{
			positions[index++] = glm::vec3(static_cast<float>(i)/10.0, static_cast<float>(j)/10.0, 0.0);
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), &positions[0], GL_STATIC_DRAW);

	auto pos_loc = glGetAttribLocation(shader_program, "pos_attrib");
	glEnableVertexAttribArray(pos_loc);
	glVertexAttribPointer(pos_loc, 3, GL_FLOAT, false, 0, 0);
	glBindVertexArray(0);
}
