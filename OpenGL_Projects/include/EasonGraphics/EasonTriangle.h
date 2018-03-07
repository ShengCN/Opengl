#pragma once
#include <vector>
#include <glm/glm.hpp>

class EasonTriangle
{
public:
	EasonTriangle();
	EasonTriangle(float*, int);
	EasonTriangle(glm::vec3, glm::vec3, glm::vec3);
	~EasonTriangle();

	void SetVertices(glm::vec3, glm::vec3, glm::vec3);
	void SetVertices(float*, int);
	std::vector<glm::vec3> GetVertices() const;

	void Rotate(float);
private:
	glm::vec3 verticeA;
	glm::vec3 verticeB;
	glm::vec3 verticeC;
	float rotation;

};

