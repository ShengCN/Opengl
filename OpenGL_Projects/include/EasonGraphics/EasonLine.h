#pragma once
#include <glm/detail/type_vec3.hpp>
#include <vector>

class EasonLine
{
public:
	EasonLine();
	EasonLine(glm::vec3, glm::vec3);
	~EasonLine();

	void Rotate(float degree);
	std::vector<glm::vec3> GetVertices() const;

private:
	glm::vec3 aPos;
	glm::vec3 bPos;
};

