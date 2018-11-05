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
	colors.clear();
	glm::vec3 center = glm::vec3(0.0f);
	float height = 1.0f, length = 0.5f;

	// prepare cube positions
	float dx = 5.0f / xSize;
	float dy = 5.0f / xSize * ySize / xSize;

	for(int j = 0; j < ySize;++j)
	{
		for(int i = 0; i < xSize; ++i)
		{
			InitCube(center + glm::vec3(i*dx - 2.5f,0.0f,j*dy - 2.5f),dx*0.6, dy*0.6, floor((1.0f - heightMap.at(xSize*j + i) / 255.0f) * 2.0), xSize*j + i);
			//InitCube(center + glm::vec3(i*dx - 2.5f, 0.0f, j*dy - 2.5f), dx, dy, 0.0f, xSize*j + i);
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

	glGenBuffers(1, &cbo);
	glBindBuffer(GL_ARRAY_BUFFER, cbo);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), &colors[0], GL_STATIC_DRAW);
	const int color_loc = glGetAttribLocation(shader_program,"color");
	glVertexAttribPointer(color_loc, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3),0);
	glEnableVertexAttribArray(color_loc);

	glBindVertexArray(0);
}

void InkPaintingVis::BufferManage()
{
}

void InkPaintingVis::ReleaseBuffers()
{
}

void InkPaintingVis::InitCube(glm::vec3 center, float xLen, float yLen, float height, int id)
{
	float color = heightMap[id];
	glm::vec3 A = center - glm::vec3(0.5*yLen, 0.0f, -0.5*yLen);
	glm::vec3 B = center + glm::vec3(0.5*yLen, 0.0f, 0.5*yLen);
	glm::vec3 C = center - glm::vec3(-0.5*yLen, 0.0f, 0.5*yLen);
	glm::vec3 D = center - glm::vec3(0.5*yLen, 0.0f, 0.5*yLen);

	// Front face
	pointPos.push_back(A); colors.push_back(glm::vec3(color)/255.0f);
	pointPos.push_back(B); colors.push_back(glm::vec3(color) / 255.0f);
	pointPos.push_back(B + glm::vec3(0.0f, height, 0.0f)); colors.push_back(glm::vec3(color) / 255.0f);
	pointPos.push_back(B + glm::vec3(0.0f, height, 0.0f)); colors.push_back(glm::vec3(color) / 255.0f);
	pointPos.push_back(A + glm::vec3(0.0f, height, 0.0f)); colors.push_back(glm::vec3(color) / 255.0f);
	pointPos.push_back(A); colors.push_back(glm::vec3(color) / 255.0f);

	// Top 
	pointPos.push_back(A + glm::vec3(0.0f, height, 0.0f)); colors.push_back(glm::vec3(color) / 255.0f);
	pointPos.push_back(B + glm::vec3(0.0f, height, 0.0f)); colors.push_back(glm::vec3(color) / 255.0f);
	pointPos.push_back(C + glm::vec3(0.0f, height, 0.0f)); colors.push_back(glm::vec3(color) / 255.0f);
	pointPos.push_back(C + glm::vec3(0.0f, height, 0.0f)); colors.push_back(glm::vec3(color) / 255.0f);
	pointPos.push_back(D + glm::vec3(0.0f, height, 0.0f)); colors.push_back(glm::vec3(color) / 255.0f);
	pointPos.push_back(A + glm::vec3(0.0f, height, 0.0f)); colors.push_back(glm::vec3(color) / 255.0f);

	// Back 
	pointPos.push_back(D); colors.push_back(glm::vec3(color) / 255.0f);
	pointPos.push_back(C); colors.push_back(glm::vec3(color) / 255.0f);
	pointPos.push_back(C + glm::vec3(0.0f, height, 0.0f)); colors.push_back(glm::vec3(color) / 255.0f);
	pointPos.push_back(C + glm::vec3(0.0f, height, 0.0f)); colors.push_back(glm::vec3(color) / 255.0f);
	pointPos.push_back(D + glm::vec3(0.0f, height, 0.0f)); colors.push_back(glm::vec3(color) / 255.0f);
	pointPos.push_back(D); colors.push_back(glm::vec3(color) / 255.0f);

	// Bottom 
	pointPos.push_back(A); colors.push_back(glm::vec3(color) / 255.0f);
	pointPos.push_back(B); colors.push_back(glm::vec3(color) / 255.0f);
	pointPos.push_back(C); colors.push_back(glm::vec3(color) / 255.0f);
	pointPos.push_back(C); colors.push_back(glm::vec3(color) / 255.0f);
	pointPos.push_back(D); colors.push_back(glm::vec3(color) / 255.0f);
	pointPos.push_back(A); colors.push_back(glm::vec3(color) / 255.0f);

	// Left 
	pointPos.push_back(A); colors.push_back(glm::vec3(color) / 255.0f);
	pointPos.push_back(A + glm::vec3(0.0f, height, 0.0f)); colors.push_back(glm::vec3(color) / 255.0f);
	pointPos.push_back(D); colors.push_back(glm::vec3(color) / 255.0f);
	pointPos.push_back(D); colors.push_back(glm::vec3(color) / 255.0f);
	pointPos.push_back(A + glm::vec3(0.0f, height, 0.0f)); colors.push_back(glm::vec3(color) / 255.0f);
	pointPos.push_back(D + glm::vec3(0.0f,height,0.0f)); colors.push_back(glm::vec3(color) / 255.0f);

	// Right
	pointPos.push_back(B); colors.push_back(glm::vec3(color) / 255.0f);
	pointPos.push_back(B + glm::vec3(0.0f, height, 0.0f)); colors.push_back(glm::vec3(color) / 255.0f);
	pointPos.push_back(C); colors.push_back(glm::vec3(color) / 255.0f);
	pointPos.push_back(C + glm::vec3(0.0f, height, 0.0f)); colors.push_back(glm::vec3(color) / 255.0f);
	pointPos.push_back(C + glm::vec3(0.0f, height, 0.0f)); colors.push_back(glm::vec3(color) / 255.0f);
	pointPos.push_back(B + glm::vec3(0.0f, height, 0.0f)); colors.push_back(glm::vec3(color) / 255.0f);
}
