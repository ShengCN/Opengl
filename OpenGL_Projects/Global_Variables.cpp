#include "Global_Variables.h"
#include "Utilities.h"

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
	billboard_vs = ".\\billboard_vs.glsl";
	billboard_gs = ".\\billboard_gs.glsl";
	billboard_fs = ".\\billboard_fs.glsl";
	fish_model = ".\\Materials\\Sphere.obj";
	billBoard_texture = ".\\Materials\\House.jpg";
	dataset_dirs.push_back(".\\Materials\\CV_Test\\1\\");
	dataset_dirs.push_back(".\\Materials\\CV_Test\\2\\");
	dataset_dirs.push_back(".\\Materials\\CV_Test\\3\\");
	dataset_dirs.push_back(".\\Materials\\CV_Test\\4\\");
	dataset_dirs.push_back(".\\Materials\\CV_Test\\5\\");
	cv_des_dir = ".\\Materials\\CV_Result\\";

	isF_MouseMovement = true;
}