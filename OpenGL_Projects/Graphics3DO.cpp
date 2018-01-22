#include "Graphics3DO.h"
#include "IchenLib/OpenGLHelpers.h"
#include "Global_Variables.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Graphics3DO::Graphics3DO()
{
}


Graphics3DO::~Graphics3DO()
{
}

void Graphics3DO::Draw()
{
	glUseProgram(shader_program);
	auto gv = Global_Variables::Instance();

	// PVM
	glm::mat4 T = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 M = T * glm::scale(glm::vec3(m_mesh_data.mScaleFactor*0.1f));
	M = glm::rotate(90.0f, glm::vec3(-1.0f, 0.0f, 0.0f)) * M;
	glm::mat4 V = gv->current_camera->GetV();
	glm::mat4 P = gv->current_camera->GetP();

	// variables
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "PVM"), 1, false, glm::value_ptr(P*V*M));
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "P"), 1, false, glm::value_ptr(P));
	glBindTexture(GL_TEXTURE_2D, m_textureId);
	glUniform1i(glGetUniformLocation(shader_program, "texture"), 0);

	glBindVertexArray(m_mesh_data.mVao);
	glDrawElements(GL_TRIANGLES, m_mesh_data.mNumIndices, GL_UNSIGNED_INT, nullptr);

	glBindVertexArray(0);
}

void Graphics3DO::Reload()
{
	if (shader_program != -1)
	{
		glDeleteProgram(shader_program);
	}

	Init_Shaders(m_vs_file, m_fs_file);
	if (m_mesh_data.mVao != -1)
	{
		Load_Model(m_mesh_file);
	}
}

void Graphics3DO::Draw_Shader_Uniforms()
{
}

void Graphics3DO::BufferManage()
{
}

void Graphics3DO::ReleaseBuffers()
{
}

void Graphics3DO::Init_Buffers()
{
}
