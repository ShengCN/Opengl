#include "GraphicsGrids.h"
#include "IchenLib/OpenGLHelpers.h"
#include "Global_Variables.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Common.h"
#include "IchenLib/Utilities.h"

GraphicsGrids::GraphicsGrids()
{
}


GraphicsGrids::~GraphicsGrids()
{
}

void GraphicsGrids::Draw()
{
	glUseProgram(shader_program);
	auto gv = Global_Variables::Instance();

	// PVM
	glm::mat4 M = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));
	//glm::mat4 M = T * glm::scale(glm::vec3(m_mesh.mScaleFactor*0.1f));
	M = glm::rotate(float_uniforms["angle"], glm::vec3(0.0f, 1.0f, 0.0f)) * M;
	glm::mat4 V = gv->current_camera->GetV();
	glm::mat4 P = gv->current_camera->GetP();

	// variables
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "PVM"), 1, false, glm::value_ptr(P*V*M));
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "P"), 1, false, glm::value_ptr(P));

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 16*3*2, GL_UNSIGNED_INT,0);

	glBindVertexArray(0);
}

void GraphicsGrids::Reload()
{
	if (shader_program != -1)
	{
		glDeleteProgram(shader_program);
	}

	Init_Shaders(m_vs_file, m_fs_file);
	Init_Buffers();
}

void GraphicsGrids::Draw_Shader_Uniforms()
{
}

void GraphicsGrids::BufferManage()
{
}

void GraphicsGrids::ReleaseBuffers()
{
}

void GraphicsGrids::Init_Buffers()
{
	std::vector<glm::vec3> gridPoistions;
	for(auto i = 0; i<5; ++i)
	{
		for(auto j= 0; j < 5;++j)
		{
			gridPoistions.push_back(glm::vec3(i, j, 0.0));
		}
	}

	// extend to -1 : +1 
	std::for_each(std::begin(gridPoistions),std::end(gridPoistions),[](glm::vec3& p)
	{
		p = (p - glm::vec3(2.0)) * 0.5f;
	});
	
	std::vector<unsigned int> gridIndices;
	
	auto rc2index = [&](int r, int c)->int
	{
		return c * 5 + r ;
	};

	for(auto i = 0; i < 16 ; ++i) // ith triangle
	{
		auto row = i % 4;
		auto col = i / 4;
		gridIndices.push_back(rc2index(row, col));
		gridIndices.push_back(rc2index(row+1, col));
		gridIndices.push_back(rc2index(row, col+1));

		gridIndices.push_back(rc2index(row+1, col+1));
		gridIndices.push_back(rc2index(row, col+1));
		gridIndices.push_back(rc2index(row+1, col));
	}

	Print_Container(gridIndices);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, gridPoistions.size()* sizeof(glm::vec3), &gridPoistions[0], GL_STATIC_DRAW);
	auto pos_loc = glGetAttribLocation(shader_program, "pos_attrib");
	glEnableVertexAttribArray(pos_loc);
	glVertexAttribPointer(pos_loc, 3, GL_FLOAT, false, 0, 0);

	// EBO
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*gridIndices.size(), &gridIndices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

