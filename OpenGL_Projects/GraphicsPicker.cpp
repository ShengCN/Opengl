#include "GraphicsPicker.h"
#include "GLCommon.h"
#include "IchenLib/Utilities.h"


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

	glBindVertexArray(m_mesh.mVao);
	// glDrawElements(GL_TRIANGLES, m_mesh.mNumIndices, GL_UNSIGNED_INT, 0);
	glDrawElementsInstanced(GL_TRIANGLES, m_mesh.mNumIndices, GL_UNSIGNED_INT, 0, 9);
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
}

void GraphicsPicker::BufferManage()
{
}

void GraphicsPicker::ReleaseBuffers()
{
}
