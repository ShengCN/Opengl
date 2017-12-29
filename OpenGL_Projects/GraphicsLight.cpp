#include "GraphicsLight.h"
#include "IchenLib/OpenGLHelpers.h"
#include "Global_Variables.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>


GraphicsLight::GraphicsLight()
{
}


GraphicsLight::~GraphicsLight()
{
}

void GraphicsLight::Init_Buffers()
{

}

void GraphicsLight::Draw()
{
	glUseProgram(shader_program);
	auto gv = Global_Variables::Instance();

	// PVM
	glm::mat4 T = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 M = T * glm::scale(glm::vec3(m_mesh_data.mScaleFactor*0.1f));
	M = glm::rotate(float_uniforms["angle"], glm::vec3(0.0f, 1.0f, 0.0f)) * M;
	glm::mat4 V = glm::lookAt(gv->vec3_uniforms["cameraPos"], gv->vec3_uniforms["cameraPos"] + glm::vec3(0.0f, 0.0f, -0.1f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 P = glm::perspective(40.0f, gv->float_uniforms["aspect"], 0.1f, 100.0f);

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

void GraphicsLight::Reload()
{
	if(shader_program != -1)
	{
		glDeleteProgram(shader_program);
	}

	Init_Shaders(m_vs_file, m_fs_file);
	if(m_mesh_data.mVao != -1)
	{
		Load_Model(m_mesh_file);
	}
}

void GraphicsLight::Draw_Shader_Uniforms()
{

}

void GraphicsLight::BufferManage()
{

}

void GraphicsLight::ReleaseBuffers()
{

}
