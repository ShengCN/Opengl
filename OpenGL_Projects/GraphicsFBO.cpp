#include "GraphicsFBO.h"
#include "GLCommon.h"
#include "Global_Variables.h"

GraphicsFBO::GraphicsFBO()
{
}


GraphicsFBO::~GraphicsFBO()
{
}

void GraphicsFBO::Draw()
{
	glUseProgram(shader_program);
	glBindVertexArray(0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void GraphicsFBO::Draw_Shader_Uniforms()
{
}

void GraphicsFBO::Reload()
{
}

void GraphicsFBO::Init_Buffers()
{
	auto gv = Global_Variables::Instance();

	// Init quad
	InitQuad(vao, vbo, ebo);

	// Frame buffer init
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, &texture_buffer);
	glBindTexture(GL_TEXTURE_2D, texture_buffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, gv->width, gv->height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_buffer, 0);

	glGenRenderbuffers(1, &render_buffer);
	glBindRenderbuffer(GL_RENDERBUFFER,render_buffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, gv->width, gv->height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, render_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GraphicsFBO::BufferManage()
{
}

void GraphicsFBO::ReleaseBuffers()
{
}
