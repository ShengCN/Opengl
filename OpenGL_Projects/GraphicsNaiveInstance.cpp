#include "GraphicsNaiveInstance.h"
#include "Global_Variables.h"


GraphicsNaiveInstance::GraphicsNaiveInstance() : instance_number(10000)
{
}


GraphicsNaiveInstance::~GraphicsNaiveInstance()
{
}

void GraphicsNaiveInstance::Draw()
{
	auto gv = Global_Variables::Instance();
	if (gv->isInstance)
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
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "P"), 1, false, glm::value_ptr(P));
	glBindTexture(GL_TEXTURE_2D, m_textureId);
	glUniform1i(glGetUniformLocation(shader_program, "texture"), 0);

	//m_mesh.Draw();
	glBindVertexArray(m_mesh.mVao);
	glBeginQuery(GL_TIME_ELAPSED, instance_timer);
	for (int i = 0; i < instance_number; ++i)
	{
		glUniformMatrix4fv(2, 1, false, glm::value_ptr(uniform_dis[i]));
		glUniform4fv(4, 1, glm::value_ptr(uniform_color[i]));
		glDrawElements(GL_TRIANGLES, m_mesh.mNumIndices, GL_UNSIGNED_INT, 0);
	}
	glEndQuery(GL_TIME_ELAPSED);
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
		glGetQueryObjectuiv(instance_timer, GL_QUERY_RESULT_AVAILABLE, &done);
	}

	glGetQueryObjectuiv(instance_timer, GL_QUERY_RESULT, &nanoseconds);
	auto name = gv->isInstance ? "Instanced rendering " : "Manually instanced ";
	//std::cout << name << "Elapsed time: " << (nanoseconds * 1e-6) << "ms" << std::endl;
	std::cout << name << " "<< nanoseconds * 1e-6 << "ms" << std::endl;
}

void GraphicsNaiveInstance::Draw_Shader_Uniforms()
{
}

void GraphicsNaiveInstance::Reload()
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

void GraphicsNaiveInstance::Init_Buffers()
{
	uniform_color.clear();
	uniform_dis.clear();
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
			auto c = static_cast<float>(i % 10) * red + static_cast<float>(j % 10) / 3 * green + blue;
			uniform_color.push_back(c);
		}
	}

	//Init displacement
	
	for (auto i = 0; i < halfdimension; ++i)
	{
		for(auto j = 0; j < halfdimension; ++j)
		{
			uniform_dis.push_back(glm::translate(glm::vec3(i - halfdimension/2, j - halfdimension/2, -50.0f)));
		}
	}

	glGenQueries(1, &instance_timer);
}

void GraphicsNaiveInstance::BufferManage()
{
}

void GraphicsNaiveInstance::ReleaseBuffers()
{
}
