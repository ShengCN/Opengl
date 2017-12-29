#pragma once
#include <glm/gtx/transform.hpp>
#include <string>
#include <unordered_map>
#include "GraphicsBase.h"
#include "GraphicsBillboard.h"

class Global_Variables
{
public:
	static Global_Variables* Instance();

	// Variables
	std::vector<GraphicsBase*> graphics;
	std::vector<GraphicsBillboard*> billboards;

	// Global variables
	std::unordered_map<std::string, float> float_uniforms;
	std::unordered_map<std::string, bool> bool_uniforms;
	std::unordered_map<std::string, int> int_uniforms;
	std::unordered_map<std::string, glm::vec3> vec3_uniforms;
	std::unordered_map<std::string, glm::vec4> vec4_uniforms;

	const std::string test_vs = ".\\vs.glsl";
	const std::string test_fs = ".\\fs.glsl";
	const std::string billboard_vs = ".\\billboard_vs.glsl";
	const std::string billboard_gs = ".\\billboard_gs.glsl";
	const std::string billboard_fs = ".\\billboard_fs.glsl";
	const std::string fish_model = ".\\Materials\\Sphere.obj";
	const std::string billBoard_texture = ".\\Materials\\House.jpg";
	const std::string dataset_dir = ".\\Materials\\Billboards\\";
	std::vector<std::string> data_files;

private:
	Global_Variables() {};
	Global_Variables(Global_Variables const&) {};
	Global_Variables& operator=(Global_Variables const&) const { return *m_instance; };

	static Global_Variables* m_instance;
};

