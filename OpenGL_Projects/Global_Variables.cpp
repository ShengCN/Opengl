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
	current_dimension = glm::vec2(0.0f, 0.0f);

	for(size_t i = 0; i < dataset_dirs.size(); ++i)
	{
		data_files[i] = Get_All_Files(dataset_dirs[i]);
		layer_anglesize_map[i] = data_files[i].size();
		delta_angle[i] = 360.0 / static_cast<float>(data_files[i].size());
	}

	delta_layer = 90.0 / static_cast<float>(dataset_dirs.size());

	current_layer = 0;
	current_angle = 0;
	layer_max = layer_anglesize_map.size();
}