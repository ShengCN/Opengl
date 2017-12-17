//#include"OpenGLHelpers.h"
//#include "imgui/imgui_impl_glut.h"
//#include <glm/glm.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <windows.h>
//#include <string>
//#include <iostream>
//#include <unordered_map>
//#include <regex>
//#include "imgui/imgui.h"
//#include "LoadMesh.h"
//#define DEBUG(x,y) std::cout<<x<<"\t"<<y<<std::endl;
//#define GET_VARIABLE_NAME(Variable) (#Variable)
//
//std::vector<std::string> allUniformNames;
//std::vector<GLenum> uniformTypes;
//bool selectedFile[1000];
//bool isMeshBtnClicked = false;
//
//extern std::unordered_map<std::string, float> float_uniforms;
//extern std::unordered_map<std::string, bool> bool_uniforms;
//extern std::unordered_map<std::string, int> int_uniforms;
//extern std::unordered_map<std::string, glm::vec3> vec3_uniforms;
//extern std::unordered_map<std::string, glm::vec4> vec4_uniforms;
//
//void getAllUniformsNames(const GLuint shader_program)
//{
//	// clear
//	allUniformNames.clear();
//	uniformTypes.clear();
//
//	GLint count, size;
//	GLenum type;
//	const GLsizei bufSize = 50;
//	GLchar name[bufSize];
//	GLsizei length;
//
//	glGetProgramiv(shader_program, GL_ACTIVE_UNIFORMS, &count);
//	for (int i = 0; i < count; ++i)
//	{
//		glGetActiveUniform(shader_program, (GLuint)i, bufSize, &length, &size, &type, name);
//		//std::cout << "Id: " << i << " U type: " << type << " Name: " << name << std::endl;
//		allUniformNames.push_back(name);
//		uniformTypes.push_back(type);
//	}
//}
//
//bool isColor(std::string name)
//{
//	std::regex rx("color");
//	std::regex rx_upper("Color");
//	auto isMatch = (std::regex_search(name, rx) || regex_search(name, rx_upper));
//
//	return isMatch;
//}
//
//void initUniformsForAutoGenrate(const GLuint shader_program)
//{
//	// clear maps
//	float_uniforms.clear();
//	bool_uniforms.clear();
//	int_uniforms.clear();
//	vec3_uniforms.clear();
//	vec4_uniforms.clear();
//
//	getAllUniformsNames(shader_program);
//	for (size_t i = 0; i < allUniformNames.size(); ++i)
//	{
//		auto uniformName = allUniformNames[i] + std::to_string(shader_program);
//	
//		switch (uniformTypes[i])
//		{
//		case 5126: // float
//			float_uniforms[uniformName] = 0.0f;
//			break;
//
//		case 5124: // int
//			int_uniforms[uniformName] = 0;
//			break;
//
//		case 35670: // bool 
//			bool_uniforms[uniformName] = false;
//			break;
//
//		case 35665:
//			vec3_uniforms[uniformName] = glm::vec3(0.0f);
//			break;
//		case 35666: // vec4
//			vec4_uniforms[uniformName] = glm::vec4(glm::vec3(0.0f),1.0f);
//			break;
//
//		default:
//			break;
//		}
//	}
//}
//
//void generateImGuiForUniforms(const GLuint shader_program, const std::string shader_name)
//{
//	glUseProgram(shader_program);
//	getAllUniformsNames(shader_program);
//
//	static bool autoDraw = true;
//	ImGui::Begin(shader_name.c_str(), &autoDraw, ImGuiWindowFlags_AlwaysAutoResize);
//	for (size_t i = 0; i < allUniformNames.size(); ++i)
//	{
//		// const auto uniformName = allUniformNames[i] + std::to_string(shader_program);
//		const auto uniformName = allUniformNames[i];
//		switch (uniformTypes[i])
//		{
//		case 5126: // float
//			ImGui::SliderFloat(uniformName.c_str(), &float_uniforms[uniformName], -20.0f, 20.0f);
//			break;
//
//		case 5124: // int
//			ImGui::SliderInt(uniformName.c_str(), &int_uniforms[uniformName], -20, 20);
//			break;
//
//		case 35670: // bool 
//			ImGui::Checkbox(uniformName.c_str(), &bool_uniforms[uniformName]);
//			break;
//
//		case 35665: // vec3
//			ImGui::SliderFloat3(uniformName.c_str(), glm::value_ptr(vec4_uniforms[uniformName]), -10.0f, 10.0f);
//			break;
//
//		case 35666: // vec4
//			isColor(uniformName)
//				? ImGui::ColorEdit4(uniformName.c_str(), glm::value_ptr(vec4_uniforms[uniformName]))
//				: ImGui::SliderFloat4(uniformName.c_str(), glm::value_ptr(vec4_uniforms[uniformName]), -10.0f, 10.0f);
//
//			break;
//
//		default:
//			break;
//		}
//	}
//	ImGui::End();
//}
//
//void updateGeneratedUniforms(const GLuint shader_program)
//{
//	getAllUniformsNames(shader_program);
//	glUseProgram(shader_program);
//
//	for (size_t i = 0; i < allUniformNames.size(); ++i)
//	{
//		const auto uniformName = allUniformNames[i].c_str();
//		//const auto keyname = allUniformNames[i] + std::to_string(shader_program);
//		const auto keyname = uniformName;
//		switch (uniformTypes[i])
//		{
//		case 5126: // float
//			glUniform1f(glGetUniformLocation(shader_program, uniformName), float_uniforms[keyname]);
//			break;
//
//		case 5124: // int
//			glUniform1i(glGetUniformLocation(shader_program, uniformName), int_uniforms[keyname]);
//			break;
//
//		case 35670: // bool 
//			glUniform1i(glGetUniformLocation(shader_program, uniformName), bool_uniforms[keyname]);
//			break;
//
//		case 35665: // vec3
//			glUniform3fv(glGetUniformLocation(shader_program, uniformName), 1, glm::value_ptr(vec3_uniforms[keyname]));
//			break;
//
//		case 35666: // vec4
//			glUniform4fv(glGetUniformLocation(shader_program, uniformName), 1, glm::value_ptr(vec4_uniforms[keyname]));
//			break;
//
//		default:
//			break;
//		}
//	}
//}
//
//void printAllUniformsNames(GLuint shader_program)
//{
//	// clear
//	allUniformNames.clear();
//	uniformTypes.clear();
//
//	GLint count, size;
//	GLenum type;
//	const GLsizei bufSize = 50;
//	GLchar name[bufSize];
//	GLsizei length;
//
//	glGetProgramiv(shader_program, GL_ACTIVE_UNIFORMS, &count);
//	for (int i = 0; i < count; ++i)
//	{
//		glGetActiveUniform(shader_program, (GLuint)i, bufSize, &length, &size, &type, name);
//		std::cout << "Id: " << i << " U type: " << type << " Name: " << name << std::endl;
//		allUniformNames.push_back(name);
//		uniformTypes.push_back(type);
//	}
//}
//
//std::vector<std::string> getAllFilesFromDirectory(std::string dirName)
//{
//	std::vector<std::string> allFiles;
//	
//	// file System
//	dirName.append("*");
//	std::wstring tmp(dirName.begin(), dirName.end());
//	WIN32_FIND_DATA FindFileData;
//	const HANDLE hFind = FindFirstFile(tmp.c_str(), &FindFileData);
//
//	if (hFind == INVALID_HANDLE_VALUE)
//	{
//		DEBUG("FindFirstFile failed (%d)\n", GetLastError());
//	}
//	do
//	{
//		if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
//		{
//			std::wstring tmp(FindFileData.cFileName);
//			const std::string str(tmp.begin(), tmp.end());
//			allFiles.push_back(str);
//		}
//	} while (FindNextFile(hFind, &FindFileData));
//	FindClose(hFind);
//
//	return allFiles;
//}
//
//bool checkSuffix(const std::string fileName, const std::string suffix)
//{
//	std::string regString = ".*\\.";
//	regString += suffix;
//
//	const std::regex regex(regString);
//	return regex_search(fileName, regex);
//}
//
//std::string filePicker(std::string dirName)
//{
//	auto files = getAllFilesFromDirectory(dirName);
//	
//	ImGui::Text("Pick mesh file: ");
//	for (size_t f = 0; f < files.size(); ++f)
//	{
//		ImGui::Selectable(files[f].c_str(), &selectedFile[f]);
//	}
//	if (ImGui::Button("Mesh Confirm"))
//	{
//		std::cout<<"Mesh File\n"; 
//		isMeshBtnClicked ^= 1;
//	}
//	if (isMeshBtnClicked)
//	{
//		isMeshBtnClicked ^= 1;
//		// change mesh
//		std::vector<int> selectedFiles;
//		for (auto i = 0; i < 1000; ++i)
//		{
//			if (selectedFile[i])
//				selectedFiles.push_back(i);
//		}
//		if (selectedFiles.size() == 1)
//		{
//			const std::string tmp = dirName +files[selectedFiles[0]];
//			DEBUG("Upload the obj: ", tmp);
//			
//			if (checkSuffix(tmp, "obj"))
//				return tmp;
//			else
//				DEBUG("It is not an obj file!", "");
//		}
//		else
//		{
//			assert("You select too many files!");
//		}
//	}
//	return "";
//}
//
//bool checkName(const std::string fileName, const std::string target)
//{
//	const std::regex regex(target);
//	return regex_search(fileName, regex);
//}
