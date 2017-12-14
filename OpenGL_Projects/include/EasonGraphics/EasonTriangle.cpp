#include "EasonTriangle.h"
#include <assert.h>
#include <glm/gtc/matrix_transform.hpp>


EasonTriangle::EasonTriangle()
{
}

EasonTriangle::EasonTriangle(float* vertices, int size)
{
	SetVertices(vertices, size);
}

EasonTriangle::EasonTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
	SetVertices(a, b, c);
}


EasonTriangle::~EasonTriangle()
{
}

void EasonTriangle::SetVertices(glm::vec3 aPos, glm::vec3 bPos, glm::vec3 cPos)
{
	verticeA = aPos;
	verticeB = bPos;
	verticeC = cPos;
}

void EasonTriangle::SetVertices(float* vertices, int size)
{
	assert(size == 9);

	verticeA = glm::vec3(vertices[0], vertices[1], vertices[2]);
	verticeB = glm::vec3(vertices[3], vertices[4], vertices[5]);
	verticeC = glm::vec3(vertices[7], vertices[7], vertices[8]);
}

std::vector<glm::vec3> EasonTriangle::GetVertices() const
{
	std::vector<glm::vec3> vertices;
	vertices.push_back(verticeA);
	vertices.push_back(verticeB);
	vertices.push_back(verticeC);

	return vertices;
}

void EasonTriangle::Rotate(float degree)
{
	auto rotateMatrix = glm::rotate(glm::mat4(), degree, glm::vec3(0.0f, 0.0f, 1.0f));
	auto aPos = glm::vec4(verticeA, 1.0f) * rotateMatrix;
	auto bPos = glm::vec4(verticeB, 1.0f) * rotateMatrix;
	auto cPos = glm::vec4(verticeC, 1.0f) * rotateMatrix;
	
	verticeA = glm::vec3(aPos.x, aPos.y, aPos.z);
	verticeB = glm::vec3(bPos.x, bPos.y, bPos.z);
	verticeC = glm::vec3(cPos.x, cPos.y, cPos.z);
}
