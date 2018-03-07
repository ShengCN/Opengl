#include "EasonLine.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

EasonLine::EasonLine()
{
}

EasonLine::EasonLine(glm::vec3 a, glm::vec3 b)
{
	aPos = a;
	bPos = b;
}


EasonLine::~EasonLine()
{
}

// Around one side vertice
void EasonLine::Rotate(float degree)
{
	//translate to origin
	//then,rotate
	auto translateMatrix = glm::translate(-aPos);
	auto rotationMatrix = glm::rotate(glm::mat4(), degree, glm::vec3(0.0f, 0.0f, 1.0f));
	auto backTranslateMatrix = glm::translate(aPos);
	auto matrix = translateMatrix * rotationMatrix * backTranslateMatrix;
	auto aP = glm::vec4(aPos, 1.0f) * matrix;
	auto bP = glm::vec4(bPos, 1.0f) * matrix;

	aPos = glm::vec3(aP.x, aP.y, aP.z);
	bPos = glm::vec3(bP.x, bP.y, bP.z);
}

std::vector<glm::vec3> EasonLine::GetVertices() const
{
	std::vector<glm::vec3> lines;
	lines.push_back(aPos);
	lines.push_back(bPos);

	return lines;
}
