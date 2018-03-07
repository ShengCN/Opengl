#include "GraphicsFish.h"
#include "Global_Variables.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "IchenLib/Utilities.h"

GraphicsFish::GraphicsFish(): instance_number(10000)
{
}


GraphicsFish::~GraphicsFish()
{
}

void GraphicsFish::Draw()
{
	auto gv = Global_Variables::Instance();
	if (!gv->isInstance)
		return;

	glUseProgram(shader_program);
	// PVM
	glm::mat4 T = glm::translate(gv->vec3_uniforms["translate"]);
	//glm::mat4 M = T * glm::scale(glm::vec3(m_mesh.mScaleFactor*0.1f));
	glm::mat4 M = T;
	M = glm::rotate(Degree2Radian(gv->float_uniforms["angle"]), glm::vec3(0.0f, 1.0f, 0.0f)) * M;
	glm::mat4 V = gv->current_camera->GetV();
	glm::mat4 P = gv->current_camera->GetP();

	// variables
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "PVM"), 1, false, glm::value_ptr(P * V * M));
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "M"), 1, false, glm::value_ptr(M));

	glBindTexture(GL_TEXTURE_2D, m_textureId);
	glUniform1i(glGetUniformLocation(shader_program, "texture"), 0);
	

	//m_mesh.Draw();
	glBindVertexArray(m_mesh.mVao);
	glDrawElements(GL_TRIANGLES, m_mesh.mNumIndices, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void GraphicsFish::Reload()
{
	if (shader_program != -1)
	{
		glDeleteProgram(shader_program);
	}

	Init_Shaders(m_vs_file, m_fs_file);

	//m_mesh.(m_mesh_file);
	Load_Model(m_mesh_file);
	ReleaseBuffers();
	Init_Buffers();
}

void GraphicsFish::Draw_Shader_Uniforms()
{
}

void GraphicsFish::BufferManage()
{
}

void GraphicsFish::ReleaseBuffers()
{
	glDeleteBuffers(1, &transform_buffer);
	glDeleteBuffers(1, &color_buffer);
}

void GraphicsFish::Init_Buffers()
{
}
