#include "GraphicsPicker.h"
#include "GLCommon.h"
#include "IchenLib/Utilities.h"
#include "Common.h"


GraphicsPicker::GraphicsPicker()
{

}


GraphicsPicker::~GraphicsPicker()
{

}

void GraphicsPicker::Draw()
{
	auto gv = Global_Variables::Instance();
	auto P = gv->current_camera->GetP();
	auto V = gv->current_camera->GetV();
	auto M = glm::rotate( Degree2Radian(gv->float_uniforms["angle"]), glm::vec3(0.0, 1.0, 0.0));
	auto PVM = P*V*M;
	glUniformMatrix4fv(0, 1, false, glm::value_ptr(PVM));
	glBindTexture(GL_TEXTURE_2D, m_textureId);
	glUniform1i(glGetUniformLocation(shader_program, "diffuse_color"), 0);

	// Pass 1
	auto pass_loc = glGetUniformLocation(shader_program, "pass");
	glUniform1i(pass_loc, 1);
	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);       //picker buffer
	glViewport(0, 0, gv->width, gv->height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(m_mesh.mVao);
	glDrawElementsInstanced(GL_TRIANGLES, m_mesh.mNumIndices, GL_UNSIGNED_INT, 0, 9);
	glBindVertexArray(0);

	// Read back the pixel to decide which object clicked
	// todo
	GLubyte buffer[4];
	glReadBuffer(GL_COLOR_ATTACHMENT0); // picker buffer
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadPixels(gv->mouseX, gv->height - gv->mouseY, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	float currentID = buffer[0];
	
	if(gv->isLBtnressed)
		DEBUG("Current ID:", currentID);

	// Pass 2
	glUniform1i(pass_loc, 2);
	glUniform1i(glGetUniformLocation(shader_program, "currentID"), static_cast<int>(currentID));
	glDrawBuffer(GL_COLOR_ATTACHMENT1);
	glViewport(0, 0, gv->width, gv->height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(m_mesh.mVao);
	glDrawElementsInstanced(GL_TRIANGLES, m_mesh.mNumIndices, GL_UNSIGNED_INT, 0, 9);
	glBindVertexArray(0);

	// Pass 3
	glUniform1i(pass_loc, 3);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_buffer);
	glUniform1i(glGetUniformLocation(shader_program, "color_tex"), 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDrawBuffer(GL_BACK);
	glViewport(0, 0, gv->width, gv->height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void GraphicsPicker::Draw_Shader_Uniforms()
{
}

void GraphicsPicker::Reload()
{
	if(shader_program!=-1)
	{
		glDeleteProgram(shader_program);
	}

	Init_Shaders(m_vs_file, m_fs_file);
	Load_Model(m_mesh_file);
	Init_Buffers();
}

void GraphicsPicker::Init_Buffers()
{
	auto gv = Global_Variables::Instance();
	glUseProgram(shader_program);

	glGenFramebuffers(1, &frame_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);

	glGenTextures(1, &picker_buffer);
	glBindTexture(GL_TEXTURE_2D, picker_buffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, gv->width, gv->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, picker_buffer, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &texture_buffer);
	glBindTexture(GL_TEXTURE_2D, texture_buffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, gv->width, gv->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, texture_buffer, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenRenderbuffers(1, &render_buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, render_buffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, gv->width, gv->height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, render_buffer);

	if( glCheckFramebufferStatus(GL_FRAMEBUFFER)!= GL_FRAMEBUFFER_COMPLETE)
	{
		DEBUG("ERROR!", "Frame buffer not complete! ");
	}
	else
	{
		DEBUG("Frame buffer complete! ","");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	InitQuad(vao, vbo, ebo);
}

void GraphicsPicker::BufferManage()
{
}

void GraphicsPicker::ReleaseBuffers()
{
}
