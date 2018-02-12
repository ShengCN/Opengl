#include "GraphicsFBO.h"
#include "GLCommon.h"
#include "Global_Variables.h"
#include <GL/freeglut.h>
#include "Common.h"
#include "IchenLib/Utilities.h"

GraphicsFBO::GraphicsFBO()
{
}


GraphicsFBO::~GraphicsFBO()
{
}

void GraphicsFBO::Draw()
{
	auto gv = Global_Variables::Instance();
	glUseProgram(shader_program);	
	
	auto P = gv->current_camera->GetP();
	auto V = gv->current_camera->GetV();
	auto M = glm::rotate( Degree2Radian(gv->float_uniforms["angle"] + 60.0f) ,glm::vec3(-1.0,0.0,0.0))*glm::rotate(3.1415f *sin(glutGet(GLUT_ELAPSED_TIME)*0.00015f), glm::vec3(0.0f, 0.0f, 1.0f));
	auto PVM = P*V*M;
	glUniformMatrix4fv(gv->PVM_loc, 1, false, glm::value_ptr(PVM));

	auto pass_loc = glGetUniformLocation(shader_program, "pass");
	// Pass 1
	glUniform1i(pass_loc, 1);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glViewport(0, 0, gv->width, gv->height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DrawSurfaceVao(sur_vao);

	// Pass 2
	glUniform1i(pass_loc, 2);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDrawBuffer(GL_BACK);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_buffer);
	glUniform1i(glGetUniformLocation(shader_program, "color_tex"), 0);
	glViewport(0, 0, gv->width, gv->height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(qua_vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void GraphicsFBO::Draw_Shader_Uniforms()
{
}

void GraphicsFBO::Reload()
{
	if (shader_program != -1)
	{
		glDeleteProgram(shader_program);
	}

	Init_Shaders(m_vs_file, m_fs_file);
	Init_Buffers();
}

void GraphicsFBO::Init_Buffers()
{
	auto gv = Global_Variables::Instance();
	glUseProgram(shader_program);
	// Init quad
	InitQuad(qua_vao, qua_vbo, qua_ebo);
	InitSurface(sur_vao, sur_vbo, sur_ebo);

	// Frame buffer init
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, &texture_buffer);
	glBindTexture(GL_TEXTURE_2D, texture_buffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, gv->width, gv->height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_buffer, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glGenRenderbuffers(1, &render_buffer);
	glBindRenderbuffer(GL_RENDERBUFFER,render_buffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, gv->width, gv->height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, render_buffer);

	// Check framebuffer
	if( glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		DEBUG("ERROR!", "Frame buffer not complete!");
	}
	else
	{
		DEBUG("Frame buffer complete!","");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GraphicsFBO::BufferManage()
{
}

void GraphicsFBO::ReleaseBuffers()
{
}
