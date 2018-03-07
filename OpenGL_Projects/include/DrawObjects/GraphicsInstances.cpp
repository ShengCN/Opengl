#include "GraphicsInstances.h"
#include "GLCommon.h"


GraphicsInstances::GraphicsInstances()
{
}


GraphicsInstances::~GraphicsInstances()
{
}

void GraphicsInstances::Draw()
{
	auto gv = Global_Variables::Instance();
	// PVM
	glm::mat4 T = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));
	//glm::mat4 M = T * glm::scale(glm::vec3(m_mesh.mScaleFactor*0.1f));
	glm::mat4 M = T;
	M = glm::rotate(gv->float_uniforms["angle"], glm::vec3(-1.0f, 0.0f, 0.0f)) * M;
	glm::mat4 V = gv->current_camera->GetV();
	glm::mat4 P = gv->current_camera->GetP();

	glUniformMatrix4fv(glGetUniformLocation(shader_program, "PVM"), 1, false, glm::value_ptr(P*V*M));
	glBindVertexArray(vao);
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 2);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void GraphicsInstances::Draw_Shader_Uniforms()
{
}

void GraphicsInstances::Reload()
{
	if (shader_program != -1)
	{
		glDeleteProgram(shader_program);
	}

	Init_Shaders(m_vs_file, m_fs_file);
}

void GraphicsInstances::Init_Buffers()
{
	std::vector<glm::vec3> positions =
	{
		glm::vec3(0.0f,0.0f,0.0f),
		glm::vec3(1.0f,1.0f,0.0f),
		glm::vec3(1.0f,0.0f,0.0f),
		glm::vec3(0.0f,1.0f,0.0f)
	};
	std::vector<unsigned int> indices =
	{
		0,1,2,
		0,3,1
	};
	std::vector<glm::vec3> offsets =
	{
		glm::vec3(0.0f,0.0f,0.0f),
		glm::vec3(0.0f,2.0f,0.0f)
	};

	std::vector<glm::vec3> colors = 
	{
		glm::vec3(1.0,0.0,0.0),
		glm::vec3(0.0,1.0,0.0),
		glm::vec3(0.0,0.0,1.0)
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), &positions[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GL_FLOAT), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribDivisor(0, 0);

	glGenBuffers(1, &instance_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, instance_buffer);
	glBufferData(GL_ARRAY_BUFFER, offsets.size() * sizeof(glm::vec3), &offsets[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribDivisor(1, 1);

	// EBO
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void GraphicsInstances::BufferManage()
{
}

void GraphicsInstances::ReleaseBuffers()
{
}
