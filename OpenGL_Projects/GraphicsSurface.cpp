#include "GraphicsSurface.h"
#include "GLCommon.h"
#define BUFFER_OFFSET(i) ((char *)NULL + (i)) 

glm::vec3 surface(float x, float y)
{
	float r = 20.0f*glm::length(glm::vec2(x, y));
	float z;
	if (r < 1e-6)
	{
		z = glm::cos(r); // lim r->0
	}
	else
	{
		z = glm::sin(r) / r;
	}

	return glm::vec3(x, z, y);
}

glm::vec3 normal(float x, float y)
{
	const float dx = 0.001f;
	const float dy = 0.001f;
	glm::vec3 du = surface(x + dx, y) - surface(x - dx, y);
	glm::vec3 dv = surface(x, y + dy) - surface(x, y - dy);
	return glm::normalize(glm::cross(du, dv));
}

GraphicsSurface::GraphicsSurface()
{
}


GraphicsSurface::~GraphicsSurface()
{
}

void GraphicsSurface::Draw()
{
	glBindVertexArray(vao);
	glDrawArrays(GL_POINTS, 0, ni*nj);
}

void GraphicsSurface::Reload() 
{

}
void GraphicsSurface::Draw_Shader_Uniforms()
{

}

void GraphicsSurface::BufferManage()
{

}

void GraphicsSurface::ReleaseBuffers()
{

}

void GraphicsSurface::Init_Buffers()
{
	int pos_loc = glGetAttribLocation(shader_program, "pos_attrib");
	int tex_coord_loc = glGetAttribLocation(shader_program, "tex_coord_attrib");
	int normal_loc = glGetAttribLocation(shader_program, "normal_attrib");

	std::vector<glm::vec3> vertices;

	for (int i = 0; i<ni; i++)
	{
		float u = i / float(ni - 1);
		float x = 2.0f*u - 1.0f;

		for (int j = 0; j<nj; j++)
		{
			float v = j / float(nj - 1);
			float y = 2.0f*v - 1.0f;

			glm::vec3 p = surface(x, y);
			vertices.push_back(p);
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(pos_loc);
	glVertexAttribPointer(pos_loc, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glBindVertexArray(0);
}