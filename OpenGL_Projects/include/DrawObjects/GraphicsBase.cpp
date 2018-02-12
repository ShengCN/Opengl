#include "GraphicsBase.h"
#include "IchenLib/IchenGlut.h"
#include "IchenLib/ShaderProcessor.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glut.h"
#include "IchenLib/LoadTexture.h"
#include "Global_Variables.h"


GraphicsBase::GraphicsBase()
{
	shader_program = -1;
}


GraphicsBase::~GraphicsBase()
{
	glDeleteProgram(shader_program);
}

GLuint GraphicsBase::Init_Shaders(const std::string vs, const std::string fs)
{
	shader_program = InitShader(vs.c_str(), fs.c_str());
	m_vs_file = vs;
	m_fs_file = fs;
	m_isGeo = false;
	return shader_program;
}

GLuint GraphicsBase::Init_Shaders(const std::string vs, const std::string gs, const std::string fs)
{
	shader_program = InitShader(vs.c_str(), gs.c_str(), fs.c_str());
	m_vs_file = vs;
	m_fs_file = fs;
	m_gs_file = gs;
	m_isGeo = true;
	return shader_program;
}

void GraphicsBase::Load_Model(const std::string model)
{
	glUseProgram(shader_program);
	m_mesh_file = model;
	m_mesh = LoadMesh(model);
	//m_mesh = Model(model);
}

void GraphicsBase::Load_Texture(const std::string t)
{
	m_texture_file = t;
	m_textureId = LoadTexture(m_texture_file, &aspect);
}

void GraphicsBase::Generate_ImGui(const std::string shader_name)
{
	glUseProgram(shader_program);
	Get_Uniforms_Names(shader_program);

	static bool autoDraw = true;
	ImGui::Begin(shader_name.c_str(), &autoDraw, ImGuiWindowFlags_AlwaysAutoResize);
	for (size_t i = 0; i < allUniformNames.size(); ++i)
	{
		// const auto uniformName = allUniformNames[i] + std::to_string(shader_program);
		const auto uniformName = allUniformNames[i];
		switch (uniformTypes[i])
		{
		case 5126: // float
			ImGui::SliderFloat(uniformName.c_str(), &float_uniforms[uniformName], -20.0f, 20.0f);
			break;

		case 5124: // int
			ImGui::SliderInt(uniformName.c_str(), &int_uniforms[uniformName], -20, 20);
			break;

		case 35670: // bool 
			ImGui::Checkbox(uniformName.c_str(), &bool_uniforms[uniformName]);
			break;

		case 35664: // vec2
			ImGui::SliderFloat2(uniformName.c_str(), glm::value_ptr(vec2_uniforms[uniformName]), -10.0f, 10.0f);
			break;

		case 35665: // vec3
			ImGui::SliderFloat3(uniformName.c_str(), glm::value_ptr(vec3_uniforms[uniformName]), -10.0f, 10.0f);
			break;

		case 35666: // vec4
			Is_Color(uniformName)
				? ImGui::ColorEdit4(uniformName.c_str(), glm::value_ptr(vec4_uniforms[uniformName]))
				: ImGui::SliderFloat4(uniformName.c_str(), glm::value_ptr(vec4_uniforms[uniformName]), -10.0f, 10.0f);
			break;

		default:
			break;
		}
	}
	ImGui::End();
}

void GraphicsBase::Update_Uniforms()
{
	Get_Uniforms_Names(shader_program);
	glUseProgram(shader_program);

	for (size_t i = 0; i < allUniformNames.size(); ++i)
	{
		const auto uniformName = allUniformNames[i].c_str();
		//const auto keyname = allUniformNames[i] + std::to_string(shader_program);
		const auto keyname = uniformName;
		GLuint loc;
		switch (uniformTypes[i])
		{
		case 5126: // float
			if(Check_Name(keyname,"time")|| Check_Name(keyname, "Time"))
			{
				float_uniforms[keyname] = Global_Variables::Instance()->float_uniforms["time"];
			}
			glUniform1f(glGetUniformLocation(shader_program, uniformName), float_uniforms[keyname]);
			break;

		case 5124: // int
			glUniform1i(glGetUniformLocation(shader_program, uniformName), int_uniforms[keyname]);
			break;

		case 35670: // bool 
			glUniform1i(glGetUniformLocation(shader_program, uniformName), bool_uniforms[keyname]);
			break;

		case 35664: // vec2
			if (Check_Name(keyname, "resolution")|| Check_Name(keyname, "Resolution"))
			{
				vec2_uniforms[keyname] = glm::vec2(Global_Variables::Instance()->width, Global_Variables::Instance()->height);
			}

			if(Check_Name(keyname,"mouse") || Check_Name(keyname,"Mouse"))
			{
				vec2_uniforms[keyname] = glm::vec2(Global_Variables::Instance()->mouseX, Global_Variables::Instance()->mouseY);
			}

			loc = glGetUniformLocation(shader_program, uniformName);
			glUniform2fv(glGetUniformLocation(shader_program, uniformName), 1, glm::value_ptr(vec2_uniforms[keyname]));
			break;

		case 35665: // vec3
			loc = glGetUniformLocation(shader_program, uniformName);
			glUniform3fv(glGetUniformLocation(shader_program, uniformName), 1, glm::value_ptr(vec3_uniforms[keyname]));
			break;

		case 35666: // vec4
			glUniform4fv(glGetUniformLocation(shader_program, uniformName), 1, glm::value_ptr(vec4_uniforms[keyname]));
			break;

		default:
			break;
		}
	}
}

void GraphicsBase::Get_Uniforms_Names(const GLuint shader_program)
{
	// clear
	allUniformNames.clear();
	uniformTypes.clear();

	GLint count, size;
	GLenum type;
	const GLsizei bufSize = 50;
	GLchar name[bufSize];
	GLsizei length;

	glGetProgramiv(shader_program, GL_ACTIVE_UNIFORMS, &count);
	for (int i = 0; i < count; ++i)
	{
		glGetActiveUniform(shader_program, (GLuint)i, bufSize, &length, &size, &type, name);
		//std::cout << "Id: " << i << " U type: " << type << " Name: " << name << std::endl;
		allUniformNames.push_back(name);
		uniformTypes.push_back(type);
	}
}
