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
	width = 1200;
	height = 800;
	windowPos = glm::vec2(20,20);
	bezier_vs = ".\\bezier_vs.glsl";
	bezier_fs = ".\\bezier_fs.glsl";
	curveFlag = false;
	pointsFlag = false;


	isLBtnressed = false;
	isRBtnPressed = false;
	lastMMovement = glm::vec2(0.0f, 0.0f);
	lastMousePos = glm::vec2(0);

	current_camera = new Camera();

	template_vs = ".\\template_vs.glsl";
	template_fs = ".\\template_fs.glsl";

	light_vs = ".\\vs.glsl";
	light_fs = ".\\fs.glsl";
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
	
	model_dir = ".\\Materials\\";
	fish_model = model_dir + "Amago0.obj";
	// fish_model = "cow.ply";
	fish_texture = model_dir + "AmagoT.bmp";
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

	picker_vs = ".\\picker_vs.glsl";
	picker_fs = ".\\picker_fs.glsl";

	particle_system_vs = ".\\particlesystem_vs.glsl";
	particle_system_fs = ".\\particlesystem_fs.glsl";

	volume_vs = ".\\volume_vs.glsl";
	volume_fs = ".\\volume_fs.glsl";

	isF_MouseMovement = true;
	isImguiOpen = true;
	isInstance = true;

	pos_attrib_loc = 0;
	texture_loc = 1;
	normal_loc = 2;

	PVM_loc = 0;
	time_loc = 1;
	resolution_loc = 2;
	mouse_loc = 3;

	PI = 3.14159265f;
}