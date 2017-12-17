//#pragma once
///***
//	  How to use auto generation: 
//		1. Declare those variables as global variables:
//			std::unordered_map<std::string, float> float_uniforms;
//			std::unordered_map<std::string, bool> bool_uniforms;
//			std::unordered_map<std::string, int> int_uniforms;
//			std::unordered_map<std::string, glm::vec3> vec3_uniforms;
//			std::unordered_map<std::string, glm::vec4> vec4_uniforms;
//           
//			You can initialize the variables by all the maps (use the uniform's name as key).
//
//		2. Call generateImGuiForUniforms(GLuint shader_program) function to generate imgui window 
//		   (Currently will generate an imgui widget called Debug).
//           Attentions: 
//			   call this function between the process of drawing ImGui.
//			   (Between ImGui_ImplGlut_NewFrame(); and ImGui::Render();)
//
//		3. Call updateGeneratedUniforms(const GLuint shader_program) in your display function (Or your own update function that will be called every frame).
//
//       If need debug, you can call void printAllUniformsNames(GLuint shader_program) 
//       to check current active uniforms.
//***/
//
//#include <GL/glew.h>
//#include <GL/gl.h>
//#include <GL/glext.h>
//#include <vector>
//#include "LoadMesh.h"
//
//void generateImGuiForUniforms(const GLuint shader_program, const std::string shader_name);
//
//void updateGeneratedUniforms(const GLuint shader_program); 
//
//void printAllUniformsNames(const GLuint shader_program);
//
//std::vector<std::string> getAllFilesFromDirectory(std::string dirName);
//
//std::string filePicker(std::string dirName);
//
//// Check if the string has the target string
//bool checkName(const std::string fileName, const std::string target);
//
//// Check if the string has the suffix
//bool checkSuffix(const std::string fileName, const std::string suffix);
