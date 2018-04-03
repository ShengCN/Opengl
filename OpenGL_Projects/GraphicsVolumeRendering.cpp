#include "GraphicsVolumeRendering.h"
#include "Global_Variables.h"
#include "IchenLib/LoadTexture.h"
#include "IchenLib/Utilities.h"

GraphicsVolumeRendering::GraphicsVolumeRendering(): vao(-1), vbo(-1), fbo(-1), fbo_texture(-1)
{
}


GraphicsVolumeRendering::~GraphicsVolumeRendering()
{
}

void GraphicsVolumeRendering::Draw()
{
	auto gv = Global_Variables::Instance();
	glUseProgram(shader_program);
	const float aspect = static_cast<float>(gv->width) / static_cast<float>(gv->height);
	glm::mat4 M = glm::scale(glm::vec3(1.0f));
	glm::mat4 V = glm::lookAt(glm::vec3(0.0f, 4.0f, 2.0f), glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f))
		* glm::rotate(Degree2Radian(gv->float_uniforms["angle"]++), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 P = glm::perspective(3.1415926f / 4.0f, aspect, 0.1f, 100.0f);
	glm::mat4 T = glm::mat4(1.0);

	const int PV_loc = 0;
	const int pass_loc = 1;
	const int tex_loc = 2;
	const int M_loc = 4;
	const int T_loc = 5;
	const int V_loc = glGetUniformLocation(shader_program, "volume_tex");

	glm::mat4 PV = P * V;
	glUniformMatrix4fv(PV_loc, 1, false, glm::value_ptr(PV));
	glUniformMatrix4fv(M_loc, 1, false, glm::value_ptr(M));
	glUniformMatrix4fv(T_loc, 1, false, glm::value_ptr(T));

	glEnable(GL_CULL_FACE);
	// Pass 1, back faces to texture to get destination for rays
	glUniform1i(pass_loc, 1);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glClear(GL_COLOR_BUFFER_BIT);
	glCullFace(GL_FRONT);
	DrawCube(vao);

	// Pass 2
	glUniform1i(pass_loc, 2);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDrawBuffer(GL_BACK);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fbo_texture);
	glUniform1i(tex_loc, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_3D, volume_texture);
	glUniform1i(V_loc, 1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_BACK);
	DrawCube(vao);
	glDisable(GL_CULL_FACE);
}

void GraphicsVolumeRendering::Draw_Shader_Uniforms()
{

}

void GraphicsVolumeRendering::Reload()
{
	if (shader_program != -1)
	{
		glDeleteProgram(shader_program);
	}

	Init_Shaders(m_vs_file, m_fs_file);
	Init_Buffers();
}

void GraphicsVolumeRendering::Init_Buffers()
{
	auto gv = Global_Variables::Instance();
	glUseProgram(shader_program);
	InitCube(vao, vbo);
	glGenTextures(1, &fbo_texture);
	glBindTexture(GL_TEXTURE_2D, fbo_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, gv->width, gv->height, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	volume_texture = LoadTexture3D(gv->volume_dir + gv->volume_data);

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_texture, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GraphicsVolumeRendering::BufferManage()
{
}

void GraphicsVolumeRendering::ReleaseBuffers()
{
}
