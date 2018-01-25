#include "Global_Variables.h"
#include "IchenLib/Utilities.h"

Global_Variables *Global_Variables::m_instance = nullptr;

Global_Variables* Global_Variables::Instance()
{
	if (!m_instance)
		m_instance = new Global_Variables();

	return m_instance;
}

Global_Variables::Global_Variables()
{
	isLBtnressed = false;
	isRBtnPressed = false;
	lastMMovement = glm::vec2(0.0f, 0.0f);
	lastMousePos = glm::vec2(0);

	current_camera = new Camera();

	test_vs = ".\\vs.glsl";
	test_fs = ".\\fs.glsl";
	grid_vs = ".\\grid_vs.glsl";
	grid_fs = ".\\grid_fs.glsl";
	billboard_vs = ".\\billboard_vs.glsl";
	billboard_gs = ".\\billboard_gs.glsl";
	billboard_fs = ".\\billboard_fs.glsl";
	light_model = ".\\Materials\\Sphere.obj";
	billBoard_texture = ".\\Materials\\House.jpg";
	dataset_dirs.push_back(".\\Materials\\CV_Test\\1\\");
	dataset_dirs.push_back(".\\Materials\\CV_Test\\2\\");
	dataset_dirs.push_back(".\\Materials\\CV_Test\\3\\");
	dataset_dirs.push_back(".\\Materials\\CV_Test\\4\\");
	dataset_dirs.push_back(".\\Materials\\CV_Test\\5\\");
	cv_des_dir = ".\\Materials\\CV_Result\\";
	
	fish_model_dir = ".\\Materials\\All\\";
	fish_model = "Bocaccio0.3ds";
	fish_texture = "BocaccT.bmp";
	fish_vs = ".\\fish_vs.glsl";
	fish_fs = ".\\fish_fs.glsl";

	particle_vs = ".\\particle_vs.glsl";
	particle_fs = ".\\particle_fs.glsl";

	isF_MouseMovement = true;
}