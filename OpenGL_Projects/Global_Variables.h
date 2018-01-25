#pragma once
#include <glm/gtx/transform.hpp>
#include <string>
#include <unordered_map>
#include "GraphicsBase.h"
#include "GraphicsBillboard.h"
#include <map>
#include "Camera.h"

class Global_Variables
{
public:
	static Global_Variables* Instance();

	// Resolution
	int width;
	int height;
	int mouseX;
	int mouseY;

	// Camera properties
	Camera* current_camera;

	// Control Varibles
	bool isLBtnressed;  // is left mouse button pressed
	bool isRBtnPressed; // is right mouse button pressed
	glm::vec2 lastMousePos;
	glm::vec2 lastMMovement; // last mouse movement

	// Variables
	std::vector<GraphicsBase*> graphics;
	std::vector<GraphicsBillboard*> billboards;

	// Global variables
	std::unordered_map<std::string, float> float_uniforms;
	std::unordered_map<std::string, bool> bool_uniforms;
	std::unordered_map<std::string, int> int_uniforms;
	std::unordered_map<std::string, glm::vec2> vec2_uniforms;
	std::unordered_map<std::string, glm::vec3> vec3_uniforms;
	std::unordered_map<std::string, glm::vec4> vec4_uniforms;

	std::string test_vs;
	std::string test_fs;
	std::string billboard_vs;
	std::string billboard_gs;
	std::string billboard_fs ;
	std::string billBoard_texture ;
	std::vector<std::string> dataset_dirs;
	std::string cv_des_dir;
	std::map<int, std::vector<std::string>> data_files;
	
	std::string light_model;
	std::string fish_model_dir;
	std::string fish_model;
	std::string fish_texture;
	std::string fish_vs;
	std::string fish_fs;
	
	// Particles
	std::string particle_vs;
	std::string particle_fs;

	// Grids
	std::string grid_vs;
	std::string grid_fs;

	// BillBoard manipulation
	std::map<int, int> layer_anglesize_map;
	int layer_max;
	glm::vec2 current_dimension;
	std::unordered_map<int,float> delta_angle;
	float delta_layer;

	int current_layer;
	int current_angle;

	// Time 
	int last_frame_time;
	int delta_time;

	// Is first Operations
	bool isF_MouseMovement;

private:
	Global_Variables();
	Global_Variables(Global_Variables const&) {};
	Global_Variables& operator=(Global_Variables const&) const { return *m_instance; };

	static Global_Variables* m_instance;
};

