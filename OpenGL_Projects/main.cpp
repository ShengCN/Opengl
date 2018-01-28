#include <iostream>
#include <GL/glew.h>
#include <windows.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include <iostream>

#include "imgui/imgui_impl_glut.h"
#include "IchenLib/IchenGlut.h"
#include "imgui/imgui.h"
#include "IchenLib/OpenGLHelpers.h"
#include "Global_Variables.h"
#include "GraphicsLight.h"
#include "IchenLib/Utilities.h"
#include "Graphics3DO.h"
#include "GraphicsPoints.h"
#include "GraphicsGrids.h"
#include "GraphicsFish.h"
#include "GraphicsInstance.h"

#define DEBUG(x,y) std::cout<<x<<"\t"<<y<<std::endl;

void Init_Global();
void ImGui_Update();

// Glut Functions
void InitOpenGL();
void Display();
void Keyboard(unsigned char key, int x, int y);
void Idle();
void MouseWheel(int butotm, int dir, int x, int y);
void ReloadShaders();

int main(int argc, char** argv)
{
	InitDefaultGlutEnvironment(argc, argv);
	InitOpenGL();

	// Register callbacks
	RegisterImGuiCallbacks();
	glutDisplayFunc(Display);
	glutKeyboardFunc(Keyboard);
	glutIdleFunc(Idle);
	glutMouseWheelFunc(MouseWheel);

	GlutMainLoop();
	return 0;
}

void ImGui_Update()
{
	auto gv = Global_Variables::Instance();

	ImGui_ImplGlut_NewFrame();
	static bool isShown = true;
	auto isBegin = ImGui::Begin("Debug", &isShown, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::ColorEdit4("Background Color", &gv->vec4_uniforms["Backgound_Color"][0]);
	ImGui::SliderFloat3("Billboard Position", &gv->vec3_uniforms["Billboard_Pos"][0], -5.0, 5.0f);
	ImGui::SliderFloat("Angle", &gv->float_uniforms["angle"], 0.0f, 360.0f);
	int i = 0;
	for (auto g : gv->graphics)
	{
		g->Generate_ImGui("test" + std::to_string(i++));
	}
	ImGui::End();
	ImGui::Render();

}

void InitOpenGL()
{
	glewInit();
	glEnable(GL_DOUBLEBUFFER);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POINT_SPRITE); // allows textured points
	glEnable(GL_PROGRAM_POINT_SIZE); //allows us to set point size in vertex shader
	glPointSize(10);

	ImGui_ImplGlut_Init();
	Init_Global();
}


void Init_Global()
{
	auto gv = Global_Variables::Instance();
	gv->vec3_uniforms["cameraPos"] = glm::vec3(0.0f, -0.2f, 3.0f);
	gv->vec3_uniforms["cameraFront"] = glm::vec3(0.0f, 0.0f, -1.0f);
	gv->vec3_uniforms["cameraUp"] = glm::vec3(0.0f, 1.0f, 0.0f);
	gv->vec3_uniforms["Billboard_Pos"] = glm::vec3(0.0f);
	gv->float_uniforms["cameraSpeed"] = 0.5f;
	gv->vec4_uniforms["Backgound_Color"] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	gv->current_camera->aspect = gv->float_uniforms["aspect"] = static_cast<float>(GetCurrentWindowWidth()) / static_cast<float>(GetCurrentWindowHeight());

	// Point Light
	GraphicsBase* point_light = new GraphicsLight();
	point_light->Init_Shaders(gv->test_vs, gv->test_fs);
	point_light->Load_Model(gv->light_model);
	point_light->vec3_uniforms["light_position"] = glm::vec3(0.0f, 100.0f, 0.0f);
	point_light->vec4_uniforms["light_color"] = glm::vec4(1.0f, 1.0f, 100.0f / 255.0f, 1.0f);
	gv->graphics.push_back(point_light);

	// Object
	GraphicsBase* fish = new GraphicsFish();
	fish->Init_Shaders(gv->fish_vs, gv->fish_fs);
	fish->Load_Model(gv->fish_model_dir + gv->fish_model);
	fish->Init_Buffers();
	fish->Load_Texture(gv->fish_model_dir + gv->fish_texture);
	gv->graphics.push_back(fish);

//	GraphicsBase* instance = new GraphicsInstance();
//	instance->Init_Shaders(gv->instance_vs, gv->instance_fs);
//	instance->Init_Buffers();
//	gv->graphics.push_back(instance);
}

void Display()
{
	auto gv = Global_Variables::Instance();
	gv->delta_time = glutGet(GLUT_ELAPSED_TIME) - gv->last_frame_time;
	gv->last_frame_time = glutGet(GLUT_ELAPSED_TIME);

	glClearColor(gv->vec4_uniforms["Backgound_Color"].x, gv->vec4_uniforms["Backgound_Color"].y,
	             gv->vec4_uniforms["Backgound_Color"].z, gv->vec4_uniforms["Backgound_Color"].a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto g : gv->graphics)
	{
		g->Update_Uniforms();
		g->Draw();
	}

	ImGui_Update();
	glutSwapBuffers();
}

void Idle()
{
	auto gv = Global_Variables::Instance();

	// Update all time variables
	const int time_ms = glutGet(GLUT_ELAPSED_TIME);
	gv->float_uniforms["time"] = 0.001f * time_ms;

	glutPostRedisplay();
}

void MouseWheel(int butotm, int dir, int x, int y)
{
	auto gv = Global_Variables::Instance();

	dir > 0
		? gv->current_camera->ProcessMouseScroll(gv->current_camera->MovementSpeed)
		: gv->current_camera->ProcessMouseScroll(-gv->current_camera->MovementSpeed);
}

void ReloadShaders()
{
	auto gv = Global_Variables::Instance();

	for (auto g : gv->graphics)
	{
		g->Reload();
	}
}

void Keyboard(unsigned char key, int x, int y)
{
	auto gv = Global_Variables::Instance();
	DEBUG("Pressed ", key);
	ImGui_ImplGlut_KeyCallback(key);

	switch (key)
	{
	case 'r':
	case 'R':
		ReloadShaders();
		break;

	case 'w':
	case 'W':
		gv->current_camera->ProcessKeyboard(Camera_Movement::FORWARD,gv->delta_time);
		gv->current_camera->Debug_Current_Pos();
		break;

	case 's':
	case 'S':
		gv->current_camera->ProcessKeyboard(Camera_Movement::BACKWARD, gv->delta_time);
		gv->current_camera->Debug_Current_Pos();
		break;

	case 'a':
	case 'A':
		gv->current_camera->ProcessKeyboard(Camera_Movement::LEFT, gv->delta_time);
		gv->current_camera->Debug_Current_Pos();
		break;

	case 'd':
	case 'D':
		gv->current_camera->ProcessKeyboard(Camera_Movement::RIGHT, gv->delta_time);
		gv->current_camera->Debug_Current_Pos();
		break;

	case 'z':
	case 'Z':
		break;

	default:
		break;
	}
}
