#pragma once
#include "DrawObjects/GraphicsBase.h"
#include "DrawObjects/IBufferManager.h"

enum class CasteljauTerminal
{
	IsFlat,
	IsInOnePixel,
	IsPolygonSmall,
	IsPolygonInOnePixel
};

class GraphicsBezier :
	public GraphicsBase
{
public:
	GraphicsBezier(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
	~GraphicsBezier();
	void InitBeizer(std::vector<glm::vec3> controlP,CasteljauTerminal m = CasteljauTerminal::IsFlat);
	void Draw() override;
	void Draw_Shader_Uniforms() override;
	void Reload() override;
	void Init_Buffers() override;
	void BufferManage() override;
	void ReleaseBuffers() override;

	void TerminalMethod(CasteljauTerminal m) { this->m = m; }
private:
	glm::mat4 m_PVM;
	GLuint vao,vbo;
	std::vector<glm::vec3> controlPoints;
	std::vector<glm::vec3> drawPoints;
	CasteljauTerminal m;

	// Tool functions
	void Casteljau(std::vector<glm::vec3> controlPoints);
	void SplitCurve(std::vector<glm::vec3> p, std::vector<glm::vec3>& left, std::vector<glm::vec3>& right);
	bool IsFlat(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);           
	bool IsInOnePixel(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
	bool IsPolygonSmall(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);  // Is polygon area small enough
	bool IsPolygonInOnePixel(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3); // Is polygon in one pixel
	inline float MSE(std::vector<float> error)
	{
		float m = 0.0f;
		for_each(error.begin(), error.end(), [&](float& n) {m += n*n; });
		return m / static_cast<float>(error.size());
	}
};

