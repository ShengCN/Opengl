#include "GraphicsLight.h"
#include "IchenLib/OpenGLHelpers.h"
#include "Global_Variables.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Common.h"


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
	glm::mat4 M = T * glm::scale(glm::vec3(m_mesh.mScaleFactor*0.1f));
	M = glm::rotate(float_uniforms["angle"], glm::vec3(0.0f, 1.0f, 0.0f)) * M;
	glm::mat4 V = gv->current_camera->GetV();
	glm::mat4 P = gv->current_camera->GetP();

//	glm::mat4 V = glm::lookAt(gv->vec3_uniforms["cameraPos"], gv->vec3_uniforms["cameraPos"] + glm::vec3(0.0f, 0.0f, -0.1f),
		//glm::vec3(0.0f, 1.0f, 0.0f));
	//glm::mat4 P = glm::perspective(40.0f, gv->float_uniforms["aspect"], 0.1f, 100.0f);

	// variables
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "PVM"), 1, false, glm::value_ptr(P*V*M));
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "P"), 1, false, glm::value_ptr(P));
	glBindTexture(GL_TEXTURE_2D, m_textureId);
	glUniform1i(glGetUniformLocation(shader_program, "texture"), 0);

	m_mesh.Draw();
	
	glBindVertexArray(0);
}

void GraphicsLight::Reload()
{
	if(shader_program != -1)
	{
		glDeleteProgram(shader_program);
	}

	Init_Shaders(m_vs_file, m_fs_file);

	// todo
//	if(m_mesh.mVao != -1)
//	{
//		Load_Model(m_mesh_file);
//	}
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
