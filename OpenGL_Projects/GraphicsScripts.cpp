#include "GraphicsScripts.h"
#include "Global_Variables.h"
#include "IchenLib/Utilities.h"
#include "Common.h"
#include "../Gui demo/include/FreeImage.h"


GraphicsScripts::GraphicsScripts()
{
}


GraphicsScripts::~GraphicsScripts()
{
}

void GraphicsScripts::Draw()
{
	auto gv = Global_Variables::Instance();
	glUseProgram(shader_program);

	glm::mat4 P = gv->current_camera->GetP();
	glm::mat4 V = gv->current_camera->GetV();
	glm::mat4 M = glm::rotate(Degree2Radian(gv->float_uniforms["angle"]), glm::vec3(0.0,1.0,0.0)) 
		*glm::rotate(Degree2Radian(90.0), glm::vec3(-1.0, 0.0, 0.0))
	*glm::scale(glm::vec3(m_mesh.mScaleFactor));


	glUniformMatrix4fv(glGetUniformLocation(shader_program, "PVM"), 1, false, glm::value_ptr(P * V * M));
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "M"), 1, false, glm::value_ptr(M));
	
	// pass 1: render to texture to save
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);       //picker buffer
	glViewport(0, 0, gv->width, gv->height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(m_mesh.mVao);
	glDrawElements(GL_TRIANGLES, m_mesh.mNumIndices, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

	// pass 2: render to screen to debug
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDrawBuffer(GL_BACK);
	glViewport(0, 0, gv->width, gv->height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(m_mesh.mVao);
	glDrawElements(GL_TRIANGLES, m_mesh.mNumIndices, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void GraphicsScripts::Draw_Shader_Uniforms()
{
}

void GraphicsScripts::Reload()
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

void GraphicsScripts::Init_Buffers()
{
	auto gv = Global_Variables::Instance();
	glUseProgram(shader_program);
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glGenTextures(1, &FBO_texture);
	glBindTextureEXT(GL_TEXTURE_2D, FBO_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, gv->width, gv->height, 0, GL_RGB, GL_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBO_texture, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, gv->width, gv->height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_COMPONENT, GL_RENDERBUFFER, RBO);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		DEBUG("ERROR!", "Frame buffer not complete! ");
	}
	else
	{
		DEBUG("Frame buffer complete! ", "");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GraphicsScripts::BufferManage()
{
}

void GraphicsScripts::ReleaseBuffers()
{
}

void GraphicsScripts::SaveScreenShot()
{

}
