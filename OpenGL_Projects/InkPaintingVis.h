#pragma once
#include "DrawObjects/GraphicsBase.h"
#include "DrawObjects/IBufferManager.h"
#include "../Gui demo/include/glm/detail/type_mat.hpp"

class InkPaintingVis :
	public GraphicsBase, public IBufferManager
{
public:
	InkPaintingVis();
	~InkPaintingVis();

	void Draw() override;
	void Draw_Shader_Uniforms() override;
	void Reload() override;
	void Init_Buffers() override;
	void BufferManage() override;
	void ReleaseBuffers() override;

	void InitSize(int x, int y, std::unordered_map<int, float> hm) 
	{
		xSize = x;
		ySize = y;
		heightMap = hm; 
	}

private:
	GLuint vao, vbo, cbo;
	int xSize, ySize;
	std::unordered_map<int, float> heightMap;
	const int X = 10;		// grid x
	const int Y = 10;		// grid y
	std::vector<glm::vec3> pointPos;
	std::vector<glm::vec3> colors;

	void InitCube(glm::vec3 center, float xLen, float yLen, float height, int id);
};

