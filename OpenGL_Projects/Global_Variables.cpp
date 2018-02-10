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

	template_vs = ".\\template_vs.glsl";
	template_fs = ".\\template_fs.glsl";

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
	
	fish_model_dir = ".\\Materials\\";
	fish_model = "Amago0.obj";
	// fish_model = "cow.ply";
	fish_texture = "AmagoT.bmp";
	fish_vs = ".\\fish_vs.glsl";
	fish_fs = ".\\fish_fs.glsl";
	naiveinstance_vs = ".\\fish_noninstance_vs.glsl";
	naiveinstance_fs = ".\\fish_noninstance_fs.glsl";

	particle_vs = ".\\particle_vs.glsl";
	particle_fs = ".\\particle_fs.glsl";

	shadertoy_vs = ".\\shadertoy_vs.glsl";
	shadertoy_fs = ".\\shadertoy_fs.glsl";
	raymarching_fs = ".\\raymarching_fs.glsl";

	framebuffer_vs = ".\\framebuffer_vs.glsl";
	framebuffer_fs = ".\\framebuffer_fs.glsl";

	isF_MouseMovement = true;
	isImguiOpen = true;
	isInstance = true;
}