#include "GraphicsInstance.h"
#include "IchenLib/OpenGLHelpers.h"
#include "Global_Variables.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

GraphicsInstance::GraphicsInstance()
{
}


GraphicsInstance::~GraphicsInstance()
{
}

void GraphicsInstance::Draw()
{
	glUseProgram(shader_program);
	auto gv = Global_Variables::Instance();

	// PVM
	auto M = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));
	auto V = gv->current_camera->GetV();
	auto P = gv->current_camera->GetP();

	glUniformMatrix4fv(0, 1, false, glm::value_ptr(P*V*M));
	glBindVertexArray(vao);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 3, 2); 
	glBindVertexArray(0);
}

void GraphicsInstance::Reload()
{
	if (shader_program != -1)
	{
		glDeleteProgram(shader_program);
	}

	Init_Shaders(m_vs_file, m_fs_file);
}

void GraphicsInstance::Draw_Shader_Uniforms()
{
}

void GraphicsInstance::BufferManage()
{
}

void GraphicsInstance::ReleaseBuffers()
{
}

void GraphicsInstance::Init_Buffers()
{
	glUseProgram(shader_program);
	// positions
	std::vector<glm::vec3> positions = { glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.5f,0.5f,0.0f),glm::vec3(0.0f,0.5f,0.0f) };

	// offsets 
	std::vector<glm::vec3> offsets = { glm::vec3(0.0f,0.0f,0.0f),glm::vec3(1.0f,0.0f,0.0f),glm::vec3(1.0f,0.0f,0.0f) ,glm::vec3(1.0f,0.0f,0.0f) };

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &offset_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*positions.size(), &positions[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Position attrib
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glVertexAttribDivisor(1, 1);

//	// Offset attrib
	glBindBuffer(GL_ARRAY_BUFFER, offset_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*offsets.size(), &offsets[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glVertexAttribDivisor(2, 1);
}
