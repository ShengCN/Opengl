#include "GraphicsFish.h"
#include "Global_Variables.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

GraphicsFish::GraphicsFish(): instance_number(10000)
{
}


GraphicsFish::~GraphicsFish()
{
}

void GraphicsFish::Draw()
{
	auto gv = Global_Variables::Instance();
	if (!gv->isInstance)
		return;

	glUseProgram(shader_program);
	// PVM
	glm::mat4 T = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));
	//glm::mat4 M = T * glm::scale(glm::vec3(m_mesh.mScaleFactor*0.1f));
	glm::mat4 M = T;
	M = glm::rotate(gv->float_uniforms["angle"], glm::vec3(-1.0f, 0.0f, 0.0f)) * M;
	glm::mat4 V = gv->current_camera->GetV();
	glm::mat4 P = gv->current_camera->GetP();

	// variables
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "PVM"), 1, false, glm::value_ptr(P * V * M));
	glBindTexture(GL_TEXTURE_2D, m_textureId);
	glUniform1i(glGetUniformLocation(shader_program, "texture"), 0);

	//m_mesh.Draw();
	glBindVertexArray(m_mesh.mVao);
	glUniform1i(3, gv->isInstance);
	if (gv->isInstance)
	{
		glBeginQuery(GL_TIME_ELAPSED, instance_timer);
		glDrawElementsInstanced(GL_TRIANGLES, m_mesh.mNumIndices, GL_UNSIGNED_INT, nullptr, instance_number);
		glEndQuery(GL_TIME_ELAPSED);
	}
	glBindVertexArray(0);
	for (int i = 0; i < 4; ++i)
	{
		glDisableVertexAttribArray(3 + i);
	}
	// Count time
	GLuint done = 0;
	GLuint nanoseconds = 0;
	while (!done)
	{
		glGetQueryObjectuiv(instance_timer,GL_QUERY_RESULT_AVAILABLE, &done);
	}

	glGetQueryObjectuiv(instance_timer, GL_QUERY_RESULT, &nanoseconds);
	auto name = gv->isInstance ? "Instanced rendering " : "Manually instanced ";
	// std::cout << name << "Elapsed time: " << (nanoseconds * 1e-6) << "ms" << std::endl;
	std::cout << name <<" "<< nanoseconds * 1e-6 << "ms" << std::endl;
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
	ReleaseBuffers();
	Init_Buffers();
}

void GraphicsFish::Draw_Shader_Uniforms()
{
}

void GraphicsFish::BufferManage()
{
}

void GraphicsFish::ReleaseBuffers()
{
	glDeleteBuffers(1, &transform_buffer);
	glDeleteBuffers(1, &color_buffer);
}

void GraphicsFish::Init_Buffers()
{
	// Init colors
	std::vector<glm::vec4> colors;
	glm::vec4 red = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 green = glm::vec4(0.0, 1.0, 0.0, 1.0);
	glm::vec4 blue = glm::vec4(0.0, 0.0, 1.0, 1.0);

	auto halfdimension = sqrt(instance_number);
	for (auto i = 0; i < halfdimension; ++i)
	{
		for (auto j = 0; j < halfdimension; ++j)
		{
			auto c = static_cast<float>(i%10) * red + static_cast<float>(j%10) / 3 * green + blue;
			colors.push_back(c);
		}
	}

	std::vector<glm::mat4> displacement;
	//Init displacement
	for (auto i = 0; i < halfdimension; ++i)
	{
		for (auto j = 0; j < halfdimension; ++j)
		{
			displacement.push_back(glm::translate(glm::vec3(i - halfdimension/2, j - halfdimension/2, -50.0f)));
		}
	}

	glBindVertexArray(m_mesh.mVao);
	glGenBuffers(1, &transform_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, transform_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * displacement.size(), &displacement[0], GL_STATIC_DRAW);
	// transfer mat4 for vertex attrib
	for (auto i = 0; i < 4; ++i)
	{
		glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * i));
		glEnableVertexAttribArray(3 + i);
		glVertexAttribDivisor(3 + i, 1);
	}

	glGenBuffers(1, &color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * colors.size(), &colors[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), nullptr);
	glVertexAttribDivisor(7, 1);

	glBindVertexArray(0);

	glGenQueries(1, &instance_timer);
}
