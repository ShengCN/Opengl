#include "IBufferManager.h"
#include "GLCommon.h"

void IBufferManager::InitQuad(GLuint& vao, GLuint& vbo, GLuint& ebo)
{
	std::vector<glm::vec3> positions = { glm::vec3(-1.0f,-1.0f,0.0f),glm::vec3(-1.0f,1.0f,0.0f) ,glm::vec3(1.0f,1.0f,0.0f),glm::vec3(1.0f,-1.0f,0.0f) };
	std::vector<unsigned int> indices = { 0,1,2,0,2,3 };
	glGenVertexArrays(1, &vao);

	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), &positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}
