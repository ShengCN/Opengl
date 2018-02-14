#include "GraphicsBezier.h"
#include "GLCommon.h"
#include "IchenLib/Utilities.h"

GraphicsBezier::GraphicsBezier(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
	: controlPoints(std::vector<glm::vec3>{p0, p1, p2, p3}),m(CasteljauTerminal::IsFlat)
{
}

GraphicsBezier::~GraphicsBezier()
{
}

void GraphicsBezier::Draw()
{
	auto gv = Global_Variables::Instance();
	glUseProgram(shader_program);
	// m_PVM
	glm::mat4 M = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));
	M = glm::rotate(Degree2Radian(gv->float_uniforms["angle"]), glm::vec3(0.0f, 1.0f, 0.0f)) * M;
	glm::mat4 V = gv->current_camera->GetV();
	glm::mat4 P = gv->current_camera->GetP();

	m_PVM = P*V*M;
	// variables
	glUniformMatrix4fv(0, 1, false, glm::value_ptr(m_PVM));


	InitBeizer(controlPoints);
	glBindVertexArray(vao);
	glDrawArrays(GL_LINE_STRIP, 0, drawPoints.size());
	glBindVertexArray(0);
}

void GraphicsBezier::Reload()
{
	if (shader_program != -1)
	{
		glDeleteShader(shader_program);
	}
	if (shader_program != -1)
	{
		glDeleteProgram(shader_program);
	}

	Init_Shaders(m_vs_file, m_fs_file);
	Init_Buffers();
}

void GraphicsBezier::Init_Buffers()
{
	glUseProgram(shader_program);
	// Init points
	InitBeizer(controlPoints);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * drawPoints.size(), &drawPoints[0], GL_DYNAMIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	glBindVertexArray(0);
}

void GraphicsBezier::BufferManage()
{
}

void GraphicsBezier::ReleaseBuffers()
{
}

void GraphicsBezier::InitBeizer(std::vector<glm::vec3> controlP, CasteljauTerminal m)
{
	if (controlP.size() < 4)
		return;

	drawPoints.clear();
	Casteljau(controlP);
	std::sort(drawPoints.begin(), drawPoints.end(), [&](glm::vec3 a, glm::vec3 b) {return a.x < b.x; });
}

void GraphicsBezier::Casteljau(std::vector<glm::vec3> controlPoints)
{
	// recursive end 
	bool result = false;
	switch(m)
	{
		case CasteljauTerminal::IsFlat:
			result = IsFlat(controlPoints[0], controlPoints[1], controlPoints[2], controlPoints[3]);
			break;
		case CasteljauTerminal::IsInOnePixel:
			result = IsInOnePixel(controlPoints[0], controlPoints[1], controlPoints[2], controlPoints[3]);
			break;
		case CasteljauTerminal::IsPolygonSmall:
			result = IsPolygonSmall(controlPoints[0], controlPoints[1], controlPoints[2], controlPoints[3]);
			break;
		case CasteljauTerminal::IsPolygonInOnePixel:
			result = IsPolygonInOnePixel(controlPoints[0], controlPoints[1], controlPoints[2], controlPoints[3]);
			break;
		default:
			break;
	}

	if (result)
	{
		drawPoints.push_back(controlPoints[0]);
		drawPoints.push_back(controlPoints[3]);
		return;
	}
	else
	{
		std::vector<glm::vec3> left(4), right(4);
		SplitCurve(controlPoints, left, right);
		Casteljau(left);
		Casteljau(right);
	}
}

void GraphicsBezier::SplitCurve(std::vector<glm::vec3> p, std::vector<glm::vec3>& left, std::vector<glm::vec3>& right)
{
	float coefficient = 0.5f;
	glm::vec3 mp11 = coefficient * (p[0] + p[1]); // middle point
	glm::vec3 mp12 = coefficient * (p[1] + p[2]);
	glm::vec3 mp13 = coefficient *(p[2] + p[3]);
	glm::vec3 mp21 = coefficient*(mp11 + mp12);
	glm::vec3 mp22 = coefficient*(mp12 + mp13);
	glm::vec3 mp31 = coefficient*(mp21 + mp22);

	// copy to left and right array
	left[0] = p[0];
	left[1] = mp11;
	left[2] = mp21;
	left[3] = mp31;
	right[0] = mp31;
	right[1] = mp22;
	right[2] = mp13;
	right[3] = p[3];
}

bool GraphicsBezier::IsFlat(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
	glm::vec3 p01 = p1 - p0; // first vector
	glm::vec3 p12 = p2 - p1; // second vector
	glm::vec3 p23 = p3 - p2; // third vector
	p01 = glm::normalize(p01);
	p12 = glm::normalize(p12);
	p23 = glm::normalize(p23);

	const auto diff12 = glm::dot(p01,p12)- 1.0f;
	const auto diff23 = glm::dot(p12, p23) - 1.0f;

	// 0.02
	return MSE(std::vector<float>{diff12, diff23}) < 1e-6;
}

bool GraphicsBezier::IsInOnePixel(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
	// coordinates before clipping
	auto clip_p0 = m_PVM * glm::vec4(p0,1.0);
	auto clip_p1 = m_PVM * glm::vec4(p1,1.0);
	auto clip_p2 = m_PVM * glm::vec4(p2,1.0);
	auto clip_p3 = m_PVM * glm::vec4(p3,1.0);

	return false;
}

bool GraphicsBezier::IsPolygonSmall(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
	return false;
}

bool GraphicsBezier::IsPolygonInOnePixel(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
	return false;
}

void GraphicsBezier::Draw_Shader_Uniforms()
{
}
