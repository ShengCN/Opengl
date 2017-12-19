#include "GraphicsBillboard.h"
#include "IchenLib/OpenGLHelpers.h"
#include "Global_Variables.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define NUM_ROWS 10
#define NUM_COLUMNS 10

GraphicsBillboard::GraphicsBillboard()
{
}


GraphicsBillboard::~GraphicsBillboard()
{
}

void GraphicsBillboard::Draw()
{
	glUseProgram(shader_program);
	auto gv = Global_Variables::Instance();

	// PVM
	glm::mat4 T = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 V = glm::lookAt(gv->vec3_uniforms["cameraPos"], gv->vec3_uniforms["cameraPos"] + glm::vec3(0.0f, 0.0f, -0.1f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 P = glm::perspective(40.0f, gv->float_uniforms["aspect"], 0.1f, 100.0f);

	// variables
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "PV"), 1, false, glm::value_ptr(P*V));
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textureId);
	glUniform1i(glGetUniformLocation(shader_program, "texture"), 0);

	glBindVertexArray(m_VAO);
	auto pos_Attrib = glGetAttribLocation(shader_program, "pos_attrib");
	glEnableVertexAttribArray(pos_Attrib);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBillboard);
	glVertexAttribPointer(pos_Attrib, 3, GL_FLOAT, GL_FALSE, 0, 0); // Position

	glDrawArrays(GL_POINTS, 0, 1);
	//glBindVertexArray(m_mesh_data.mVao);
	//glDrawElements(GL_TRIANGLES, m_mesh_data.mNumIndices, GL_UNSIGNED_INT, nullptr);

//	glDisableVertexAttribArray(0);
}

void GraphicsBillboard::Reload()
{
	if (shader_program != -1)
	{
		glDeleteProgram(shader_program);
	}

	m_isGeo? 
		Init_Shaders(m_vs_file,m_gs_file, m_fs_file): Init_Shaders(m_vs_file, m_fs_file);

	Load_Texture(m_texture_file);
}

void GraphicsBillboard::Draw_Shader_Uniforms()
{
}

void GraphicsBillboard::BufferManage()
{
}

void GraphicsBillboard::ReleaseBuffers()
{
	glDeleteBuffers(1, &m_VBillboard);
}

void GraphicsBillboard::Init_Buffers()
{
	auto gv = Global_Variables::Instance();

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBillboard);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBillboard);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3), &gv->vec3_uniforms["Billboard_Pos"][0], GL_STATIC_DRAW);
}
