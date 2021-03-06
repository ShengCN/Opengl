#include "IBufferManager.h"
#include "GLCommon.h"
const int ni = 250; //number of points in each direction
const int nj = 250;

glm::vec3 IBufferManager::surface(float x, float y)
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

	return glm::vec3(x, y, z);
}

glm::vec3 IBufferManager::normal(float x, float y)
{
	const float dx = 0.001f;
	const float dy = 0.001f;
	glm::vec3 du = surface(x + dx, y) - surface(x - dx, y);
	glm::vec3 dv = surface(x, y + dy) - surface(x, y - dy);
	return glm::normalize(glm::cross(du, dv));
}


void IBufferManager::InitQuad(GLuint& vao, GLuint& vbo, GLuint& ebo)
{
	std::vector<glm::vec3> positions = { glm::vec3(-1.0f,-1.0f,0.0f),glm::vec3(-1.0f,1.0f,0.0f) ,glm::vec3(1.0f,1.0f,0.0f),glm::vec3(1.0f,-1.0f,0.0f) };
	std::vector<unsigned int> indices = { 0,1,2,0,2,3 };
	glGenVertexArrays(1, &vao);

	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), &positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void IBufferManager::InitSurface(GLuint& vao, GLuint& vbo, GLuint& ebo)
{
	const int pos_loc = 0;
	const int tex_coord_loc = 1;
	const int normal_loc = 2;

	std::vector<float> vertices;

	for (int i = 0; i<ni; i++)
	{
		float u = i / float(ni - 1);
		float x = 2.0f*u - 1.0f;

		for (int j = 0; j<nj; j++)
		{
			float v = j / float(nj - 1);
			float y = 2.0f*v - 1.0f;

			glm::vec3 p = surface(x, y);

			//push vertex coords
			vertices.push_back(p.x);
			vertices.push_back(p.y);
			vertices.push_back(p.z);

			//push tex coords
			vertices.push_back(u);
			vertices.push_back(v);

			glm::vec3 n = normal(x, y);
			//push vertex coords
			vertices.push_back(n.x);
			vertices.push_back(n.y);
			vertices.push_back(n.z);

		}
	}

	const int stride = 8 * sizeof(float);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(pos_loc);
	glVertexAttribPointer(pos_loc, 3, GL_FLOAT, GL_FALSE, stride, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(tex_coord_loc);
	glVertexAttribPointer(tex_coord_loc, 2, GL_FLOAT, GL_FALSE, stride, BUFFER_OFFSET(3 * sizeof(float)));

	glEnableVertexAttribArray(normal_loc);
	glVertexAttribPointer(normal_loc, 3, GL_FLOAT, GL_FALSE, stride, BUFFER_OFFSET(5 * sizeof(float)));

	std::vector<unsigned int> indices;

	for (int i = 0; i<ni - 1; i++)
	{
		for (int j = 0; j<nj; j++)
		{
			indices.push_back(i*nj + j);
			indices.push_back(i*nj + j + nj);
		}
		indices.push_back(ni*nj);
	}

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

// Attention: pos_loc attribute should have layout = 0
void IBufferManager::InitCube(GLuint& vao, GLuint& vbo)
{
	const float cube_verts[] = { -1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f };

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_verts), &cube_verts[0], GL_STATIC_DRAW);
	
	const GLint pos_loc = 0;
	glEnableVertexAttribArray(pos_loc);
	glVertexAttribPointer(pos_loc, 3, GL_FLOAT, false, 0, 0);
	glBindVertexArray(0);
}

void IBufferManager::DrawSurfaceVao(GLuint vao)
{
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(ni*nj);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLE_STRIP, (ni - 1) * 2 * nj + (ni - 1), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void IBufferManager::DrawQuad(GLuint vao)
{
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void IBufferManager::DrawCube(GLuint vao)
{
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}
