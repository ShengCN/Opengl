#pragma once
#include <glm/gtx/transform.hpp>
#include <string>
#include <unordered_map>
#include "GraphicsBase.h"
#include "GraphicsBillboard.h"
#include "CV_Lib.h"

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

	std::string test_vs;
	std::string test_fs;
	std::string billboard_vs;
	std::string billboard_gs;
	std::string billboard_fs ;
	std::string fish_model ;
	std::string billBoard_texture ;
	std::string dataset_dir;
	std::string cv_des_dir;
	std::vector<std::string> data_files;

private:
	Global_Variables();
	Global_Variables(Global_Variables const&) {};
	Global_Variables& operator=(Global_Variables const&) const { return *m_instance; };

	static Global_Variables* m_instance;

	
};

