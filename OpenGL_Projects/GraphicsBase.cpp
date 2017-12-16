#include "GraphicsBase.h"
#include "IchenLib/IchenGlut.h"
#include "IchenLib/ShaderProcessor.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>


GraphicsBase::GraphicsBase()
{
	shader_program = -1;
}


GraphicsBase::~GraphicsBase()
{
	glDeleteProgram(shader_program);
}

bool GraphicsBase::Init_Shaders(const std::string vs, const std::string fs)
{
	shader_program = InitShader(vs.c_str(), fs.c_str());
	return shader_program;
}

bool GraphicsBase::Init_Shaders(const std::string vs, const std::string gs, const std::string fs)
{
	shader_program = InitShader(vs.c_str(),gs.c_str(), fs.c_str());
	return shader_program;
}

void GraphicsBase::Load_Model(const std::string model)
{
	mesh_data = LoadMesh(model);
}
