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

	inline void TerminalMethod(CasteljauTerminal m) { this->m = m; }
	inline void AddControlPointC1()					{ glm::vec3 h = *controlPoints.rbegin() - *(controlPoints.rbegin() + 1); controlPoints.push_back(*controlPoints.rbegin() + h);}
	inline void AddControlPointRandom()				{ controlPoints.push_back(glm::vec3(random11(), random11(), random11())*3.0f); }
	inline void DeleteControlPoint() { if (controlPoints.size() > 4) controlPoints.erase(controlPoints.begin() + controlPoints.size() -1); }
private:
	glm::mat4 m_PVM;
	GLuint vao[2],vbo[2];
	std::vector<glm::vec3> controlPoints;
	std::vector<glm::vec3> drawPoints;
	CasteljauTerminal m;

	// Tool functions
	void Casteljau(std::vector<glm::vec3> controlPoints); // only draw the first four points
	void SplitCurve(std::vector<glm::vec3> p, std::vector<glm::vec3> left, std::vector<glm::vec3> right);
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

	//returns random number from <-1,1>
	inline float random11() {
		return 2.f*rand() / (float)RAND_MAX - 1.f;
	}

	inline bool floatEqual(float a, float b) { return abs(a - b) < 1e-4; }
	inline bool floatEqualV(glm::vec2 a, glm::vec2 b) { return  floatEqual(a.x, b.x) && floatEqual(a.y, b.y); }
};

