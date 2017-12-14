#include "GrahicsGenerator.h"



GrahicsGenerator::GrahicsGenerator()
{
}


GrahicsGenerator::~GrahicsGenerator()
{
}

EasonTriangle GrahicsGenerator::GenerateTriangle(float length)
{
	if (length <= 0)
		return EasonTriangle(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	float halfWidth = length/2;
	float halfHeight = length /2;

	glm::vec3 aPos = glm::vec3(-halfWidth, -halfHeight, 0.0f);
	glm::vec3 bPos = glm::vec3(halfWidth, -halfHeight, 0.0f);
	glm::vec3 cPos = glm::vec3(0.0f, halfHeight, 0.0f);

	return EasonTriangle(aPos, bPos, cPos);
}
