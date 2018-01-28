#include "GraphicsFish.h"
#include "IchenLib/OpenGLHelpers.h"
#include "Global_Variables.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

GraphicsFish::GraphicsFish()
{
}


GraphicsFish::~GraphicsFish()
{
}

void GraphicsFish::Draw()
{
	glUseProgram(shader_program);
	auto gv = Global_Variables::Instance();

	// PVM
	glm::mat4 T = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));
	//glm::mat4 M = T * glm::scale(glm::vec3(m_mesh.mScaleFactor*0.1f));
	glm::mat4 M = T;
	M = glm::rotate(gv->float_uniforms["angle"], glm::vec3(-1.0f, 0.0f, 0.0f)) * M;
	glm::mat4 V = gv->current_camera->GetV();
	glm::mat4 P = gv->current_camera->GetP();

	// variables
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "PVM"), 1, false, glm::value_ptr(P*V*M));
	glBindTexture(GL_TEXTURE_2D, m_textureId);
	glUniform1i(glGetUniformLocation(shader_program, "texture"), 0);

	//m_mesh.Draw();
	glBindVertexArray(m_mesh.mVao);
	glBindBuffer(GL_ARRAY_BUFFER, position_vbo);
	// glDrawElements(GL_TRIANGLES, m_mesh.mNumIndices, GL_UNSIGNED_INT, 0);
	glDrawElementsInstanced(GL_TRIANGLES, m_mesh.mNumIndices, GL_UNSIGNED_INT, nullptr , 3);

	glBindVertexArray(0);
}

void GraphicsFish::Reload()
{
	if (shader_program != -1)
	{
		glDeleteProgram(shader_program);
	}

	Init_Shaders(m_vs_file, m_fs_file);

	//m_mesh.(m_mesh_file);
	Load_Model(m_mesh_file);
}

void GraphicsFish::Draw_Shader_Uniforms()
{
}

void GraphicsFish::BufferManage()
{
}

void GraphicsFish::ReleaseBuffers()
{
}

void GraphicsFish::Init_Buffers()
{
	glUseProgram(shader_program);
	// Init colors
	std::vector<glm::vec4> colors;
	glm::vec4 red = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 green = glm::vec4(0.0, 1.0, 0.0, 1.0);
	glm::vec4 blue = glm::vec4(0.0, 0.0, 1.0, 1.0);

	for (auto i = 0; i < 3; ++i)
	{
		for (auto j = 0; j < 3; ++j)
		{
			auto c = static_cast<float>(i) / 3 * red + static_cast<float>(j) / 3 * green + blue;
			colors.push_back(c);
		}
	}

	// Init positions
	std::vector<glm::vec3> positions;
	float scale = 0.3f;
	positions.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	for(auto i = 0; i < 3;++i)
	{
		for(auto j = 0; j < 3; ++j)
		{
			positions.push_back(glm::vec3(1.0f,0.0f,0.0f));
		}
	}
	
	//glBindVertexArray(m_mesh.mVao);

	glGenBuffers(1, &position_vbo);
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, position_vbo);
	glBufferData(position_vbo, sizeof(glm::vec3)*positions.size(), &positions[0], GL_STATIC_DRAW);
	glVertexAttribPointer(3, 3, GL_FLOAT, false, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(3, 1);

//	glGenBuffers(1, &position_vbo);
//	glBindBuffer(GL_ARRAY_BUFFER, position_vbo);
//	glBufferData(position_vbo, sizeof(glm::vec3)*positions.size(), &positions[0], GL_STATIC_DRAW);
//	glEnableVertexAttribArray(4);
//	glVertexAttribPointer(4, 3, GL_FLOAT, false, 0, nullptr);
//	glVertexAttribDivisor(4, 1);
}
