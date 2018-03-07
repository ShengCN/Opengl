#pragma once
#include <gl/glew.h>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include "IchenLib/LoadMesh.h"
#include <unordered_map>
#include <regex>

class GraphicsBase
{
public:
	GraphicsBase();
	virtual ~GraphicsBase();

	GLuint Init_Shaders(const std::string vs, const std::string fs);
	GLuint Init_Shaders(const std::string vs, const std::string gs, const std::string fs);
	GLuint Init_Shaders_TransformFeedback(const std::string vs, const std::string fs, const char *vars[], unsigned int size); // Tansform feedback

	void Load_Model(const std::string model);
	void Load_Texture(const std::string t);

	// imgui auto generation
	void Generate_ImGui(const std::string shader_name);
	void Update_Uniforms();

	// Get && Set
	GLint Get_Shader() const { return shader_program; }

	virtual void Draw() = 0;
	virtual void Draw_Shader_Uniforms() = 0;
	virtual void Reload() = 0;
	
	virtual void Init_Buffers() = 0;
	virtual void BufferManage() = 0;
	virtual void ReleaseBuffers() = 0;

	// ImGui Autogenerated
	std::unordered_map<std::string, float> float_uniforms;
	std::unordered_map<std::string, bool> bool_uniforms;
	std::unordered_map<std::string, int> int_uniforms;
	std::unordered_map<std::string, glm::vec2> vec2_uniforms;
	std::unordered_map<std::string, glm::vec3> vec3_uniforms;
	std::unordered_map<std::string, glm::vec4> vec4_uniforms;

protected:
	GLint shader_program;
	std::string m_vs_file;
	std::string m_gs_file;
	std::string m_fs_file;
	MeshData m_mesh;
	std::string m_mesh_file;
	GLuint m_textureId;
	std::string m_texture_file;
	bool m_isGeo;
	float aspect;

private:
	std::vector<std::string> allUniformNames;
	std::vector<GLenum> uniformTypes;
	

	void  Get_Uniforms_Names(const GLuint shader_program);
	bool Is_Color(std::string name)
	{
		std::regex rx("color");
		std::regex rx_upper("Color");
		return (std::regex_search(name, rx) || regex_search(name, rx_upper));
	}

	// Check if filename has target name
	bool Check_Name(const std::string fileName, const std::string target)
	{
		const std::regex regex(target);
		return regex_search(fileName, regex);
	}
};

