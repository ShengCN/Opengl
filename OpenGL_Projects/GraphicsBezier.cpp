#include "GraphicsBezier.h"
#include "GLCommon.h"
#include "IchenLib/Utilities.h"
#include "Common.h"

GraphicsBezier::GraphicsBezier(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
	: controlPoints(std::vector<glm::vec3>{p0, p1, p2, p3}),m(CasteljauTerminal::IsFlat),isFirstP(true),lastControlPointID(-1), recursiveTimes(0)
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

	// dynamically change control points
	int pass = -1;
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);       // picker buffer
	glViewport(0, 0, gv->width, gv->height);
	glClear(GL_COLOR_BUFFER_BIT);
	DrawControlPolygons(pass);  // pass == -1 or 2

	GLubyte buffer[4];
	glReadBuffer(GL_COLOR_ATTACHMENT0); // picker buffer
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadPixels(gv->mouseX, gv->height - gv->mouseY, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	int currentID = static_cast<int>(floor(buffer[0]/25.0));
	// DEBUG("Current ID:", currentID);

	if(gv->isLBtnressed)
	{
		if (isFirstP)
		{
			isFirstP = !isFirstP;
			lastMousePos = glm::vec2(gv->mouseX,gv->mouseY);
			lastControlPointID = currentID;
		}
		else
		{
			offset = glm::vec2(gv->lastMousePos - lastMousePos);
			offset.y = -offset.y;
		}
	}
	else
	{
		// DEBUG(offset.x, offset.y);
		if (lastControlPointID <= controlPoints.size() - 1 && lastControlPointID >= 0)
			controlPoints[lastControlPointID] += glm::vec3(offset, 0.0) * (gv->current_camera->Right + gv->current_camera->Up) * 0.015f;

		isFirstP = true;
		lastControlPointID = -1;
		offset = glm::vec2(0.0);
	}

	// Update Beizer
	InitBeizer(controlPoints);
	DEBUG("Control points : ", controlPoints.size());
	DEBUG("Draw points : ", drawPoints.size());

	// Draw beizer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDrawBuffer(GL_BACK);
	glViewport(0, 0, gv->width, gv->height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DrawBezier();

	// Draw control points
	DrawControlPolygons(2);
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
	auto gv = Global_Variables::Instance();

	// dynamically change control points
	InitBeizer(controlPoints);

	// Bezier curve
	glGenVertexArrays(2, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(3, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * drawPoints.size(), &drawPoints[0], GL_DYNAMIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Control points
	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*controlPoints.size(), &controlPoints[0], GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Ids
	float id = 0.0f;
	std::vector<float> ids(controlPoints.size());
	std::for_each(ids.begin(), ids.end(), [&](float& i) {i = id++; });
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*ids.size(), &ids[0], GL_DYNAMIC_DRAW);
	auto id_loc = glGetAttribLocation(shader_program, "id");
	glEnableVertexAttribArray(id_loc);
	glVertexAttribPointer(id_loc, 1, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);

	// frame buffer object
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, &tbo);
	glBindTexture(GL_TEXTURE_2D, tbo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, gv->width, gv->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_CLAMP);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tbo, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		DEBUG("ERROR!", "Frame buffer not complete! ");
	}
	else
	{
		DEBUG("Frame buffer complete! ", "");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

	// if control points are larger than 4, draw new curve for the new points
	if(controlP.size()>4)
	{
		for(unsigned int i = 0; i < (controlP.size()-4)/3;++i)
		{
			auto beginIndex = (i + 1) * 3;
			Casteljau(std::vector<glm::vec3>(controlP.begin() + beginIndex, controlP.begin() + beginIndex + 4));
		}
	}

	// std::sort(drawPoints.begin(), drawPoints.end(), [&](glm::vec3 a, glm::vec3 b) {return a.x < b.x; });
}

void GraphicsBezier::Casteljau(std::vector<glm::vec3> p)
{
	// recursive end 
	bool result = false;
	switch(m)
	{
		case CasteljauTerminal::IsFlat:
			result = IsFlat(p[0], p[1], p[2], p[3]);
			break;
		case CasteljauTerminal::IsInOnePixel:
			result = IsInOnePixel(p[0], p[1], p[2], p[3]);
			break;
		case CasteljauTerminal::IsPolygonSmall:
			result = IsPolygonSmall(p[0], p[1], p[2], p[3]);
			break;
		case CasteljauTerminal::IsPolygonInOnePixel:
			result = IsPolygonInOnePixel(p[0], p[1], p[2], p[3]);
			break;
		default:
			break;
	}

	if (result)
	{
		drawPoints.push_back(p[0]);
		drawPoints.push_back(p[3]);
		// DEBUG("Current points: ", drawPoints.size());
		return;
	}
	else
	{
		std::vector<glm::vec3> left(4), right(4);
		SplitCurve(p, left, right);
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

	return MSE(std::vector<float>{diff12, diff23}) < 1e-5;
}

bool GraphicsBezier::IsInOnePixel(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
	auto gv = Global_Variables::Instance();
	// coordinates before clipping
	auto clip_p0 = m_PVM * glm::vec4(p0,1.0);
	auto clip_p1 = m_PVM * glm::vec4(p1,1.0);
	auto clip_p2 = m_PVM * glm::vec4(p2,1.0);
	auto clip_p3 = m_PVM * glm::vec4(p3,1.0);

	// division to NDC
	clip_p0 /= clip_p0.w;
	clip_p1 /= clip_p1.w;
	clip_p2 /= clip_p2.w;
	clip_p3 /= clip_p3.w;

	// map to pixel space, only need to calculate x, y
	auto map2pixel = [&](glm::vec4 p)->glm::vec2
	{
		glm::vec2 r = glm::vec2((0.5*p.x + 0.5) * gv->width, (0.5*p.y+0.5)*gv->height);
		return r;
	};

	glm::vec2 pixel_p0 = map2pixel(clip_p0);
	glm::vec2 pixel_p1 = map2pixel(clip_p1);
	glm::vec2 pixel_p2 = map2pixel(clip_p2);
	glm::vec2 pixel_p3 = map2pixel(clip_p3);

	auto checkPair = [&](glm::vec2 a,glm::vec2 b)->bool
	{
		float epison = static_cast<float>(1e-4);
		return glm::floor(a) == glm::floor(b) ||
			glm::floor(a + epison) == glm::floor(b) ||
			glm::floor(a) == glm::floor(b + epison);
	};

	// assume in an integer step is in one pixel
	bool isOne =  checkPair(pixel_p0, pixel_p1) &&
		checkPair(pixel_p1,pixel_p2)&&
		checkPair(pixel_p2,pixel_p3);

	return isOne;
}

bool GraphicsBezier::IsPolygonSmall(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
	// calculate area
	glm::vec3 v1 = p1 - p0;
	glm::vec3 v2 = p2 - p1;
	glm::vec3 v3 = p2 - p0;
	glm::vec3 v4 = p3 - p2;
	auto area = 0.5*(glm::length(glm::cross(v1, v2)) + glm::length(glm::cross(v3, v4)));

	return area < 1e-6;
}

bool GraphicsBezier::IsPolygonInOnePixel(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
	auto gv = Global_Variables::Instance();
	// calculate area
	glm::vec3 v1 = p1 - p0;
	glm::vec3 v2 = p2 - p1;
	glm::vec3 v3 = p2 - p0;
	glm::vec3 v4 = p3 - p2;
	auto area = 0.5*(glm::length(glm::cross(v1, v2)) + glm::length(glm::cross(v3, v4)));

	return area < 1.0/ static_cast<double>(gv->width * gv->height);
}

void GraphicsBezier::DrawBezier()
{
	glUniform1i(glGetUniformLocation(shader_program, "pass"), 1);
	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, drawPoints.size() * sizeof(glm::vec3), &drawPoints[0], GL_DYNAMIC_DRAW);
	glDrawArrays(GL_LINES, 0, drawPoints.size());
	glBindVertexArray(0);
}

void GraphicsBezier::DrawControlPolygons(int pass)
{
	auto gv = Global_Variables::Instance();
//	if (gv->pointsFlag)
//	{
		glUniform1i(glGetUniformLocation(shader_program, "pass"), pass);
		glBindVertexArray(vao[1]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, controlPoints.size() * sizeof(glm::vec3), &controlPoints[0], GL_DYNAMIC_DRAW);
		
		int id = 0;
		std::vector<float> ids(controlPoints.size());
		std::for_each(ids.begin(), ids.end(), [&](float& i) {i = static_cast<float>(id++); });
		glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*ids.size(), &ids[0], GL_DYNAMIC_DRAW);
		
		glDrawArrays(GL_POINTS, 0, controlPoints.size());

		// Draw control points polygon 
		if (pass != -1)
		{
			glUniform1i(glGetUniformLocation(shader_program, "pass"), 3);
			glDrawArrays(GL_LINE_STRIP, 0, controlPoints.size());
			glBindVertexArray(0);
		}
	//}
}

void GraphicsBezier::Draw_Shader_Uniforms()
{
}
