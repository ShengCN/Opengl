#include "InkPaintingVis.h"
#include "Global_Variables.h"
#include "IchenLib/Utilities.h"

InkPaintingVis::InkPaintingVis():xSize(-1),ySize(-1)
{
}


InkPaintingVis::~InkPaintingVis()
{
}

void InkPaintingVis::Draw()
{
	auto gv = Global_Variables::Instance();
	glUseProgram(shader_program);
	
	auto P = gv->current_camera->GetP();
	auto V = gv->current_camera->GetV();
	auto M = glm::rotate(Degree2Radian(gv->float_uniforms["angle"]),glm::vec3(0.0f,1.0f,0.0f));
	const int PVM_loc = glGetUniformLocation(shader_program, "PVM");
	glUniformMatrix4fv(PVM_loc, 1, false, glm::value_ptr(P*V*M));
	
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, pointPos.size());
	glBindVertexArray(0);
}

void InkPaintingVis::Draw_Shader_Uniforms()
{
}

void InkPaintingVis::Reload()
{
	if (shader_program != -1) glDeleteProgram(shader_program);

	Init_Shaders(m_vs_file, m_fs_file);
	Init_Buffers();
}

void InkPaintingVis::Init_Buffers()
{
	if (xSize < 0 || ySize < 0)
		return;

	glUseProgram(shader_program);
	pointPos.clear();
	glm::vec3 center = glm::vec3(0.0f);
	float height = 1.0f, length = 0.5f;

	// prepare cube positions
	for(int i = 0; i < xSize; ++i)
	{
		for(int j = 0; j < ySize; ++j)
		{
			InitCube(center + glm::vec3(i-2.5f, 0.0f, j- 2.5f),length, heightMap.at(i+j));
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, pointPos.size() * sizeof(glm::vec3), &pointPos[0], GL_STATIC_DRAW);

	const int pos_loc = 0;
	glVertexAttribPointer(pos_loc, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	glEnableVertexAttribArray(pos_loc);

	glBindVertexArray(0);
}

void InkPaintingVis::BufferManage()
{
}

void InkPaintingVis::ReleaseBuffers()
{
}

void InkPaintingVis::InitCube(glm::vec3 center, float length, float height)
{
	glm::vec3 A = center - glm::vec3(0.5*length, 0.0f, -0.5*length);
	glm::vec3 B = center + glm::vec3(0.5*length, 0.0f, 0.5*length);
	glm::vec3 C = center - glm::vec3(-0.5*length, 0.0f, 0.5*length);
	glm::vec3 D = center - glm::vec3(0.5*length, 0.0f, 0.5*length);

	// Front face
	pointPos.push_back(A);
	pointPos.push_back(B);
	pointPos.push_back(B + glm::vec3(0.0f, height, 0.0f));
	pointPos.push_back(B + glm::vec3(0.0f, height, 0.0f));
	pointPos.push_back(A + glm::vec3(0.0f, height, 0.0f));
	pointPos.push_back(A);

	// Top 
	pointPos.push_back(A + glm::vec3(0.0f, height, 0.0f));
	pointPos.push_back(B + glm::vec3(0.0f, height, 0.0f));
	pointPos.push_back(C + glm::vec3(0.0f, height, 0.0f));
	pointPos.push_back(C + glm::vec3(0.0f, height, 0.0f));
	pointPos.push_back(D + glm::vec3(0.0f, height, 0.0f));
	pointPos.push_back(A + glm::vec3(0.0f, height, 0.0f));

	// Back 
	pointPos.push_back(D);
	pointPos.push_back(C);
	pointPos.push_back(C + glm::vec3(0.0f, height, 0.0f));
	pointPos.push_back(C + glm::vec3(0.0f, height, 0.0f));
	pointPos.push_back(D + glm::vec3(0.0f, height, 0.0f));
	pointPos.push_back(D);

	// Bottom 
	pointPos.push_back(A);
	pointPos.push_back(B);
	pointPos.push_back(C);
	pointPos.push_back(C);
	pointPos.push_back(D);
	pointPos.push_back(A);

	// Left 
	pointPos.push_back(A);
	pointPos.push_back(A + glm::vec3(0.0f, height, 0.0f));
	pointPos.push_back(D);
	pointPos.push_back(D);
	pointPos.push_back(A + glm::vec3(0.0f, height, 0.0f));
	pointPos.push_back(D + glm::vec3(0.0f,height,0.0f));

	// Right
	pointPos.push_back(B);
	pointPos.push_back(B + glm::vec3(0.0f, height, 0.0f));
	pointPos.push_back(C);
	pointPos.push_back(C + glm::vec3(0.0f, height, 0.0f));
	pointPos.push_back(C + glm::vec3(0.0f, height, 0.0f));
	pointPos.push_back(B + glm::vec3(0.0f, height, 0.0f));
}
