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
#include <stdio.h>
#include <iostream>

#include "imgui/imgui_impl_glut.h"
#include "IchenLib/IchenGlut.h"
#include "imgui/imgui.h"
#include "IchenLib/OpenGLHelpers.h"
#include "Global_Variables.h"
#include "GraphicsLight.h"
#include "GraphicsBillboard.h"
#include "Utilities.h"
#include "CV_Lib.h"

#define DEBUG(x,y) std::cout<<x<<"\t"<<y<<std::endl;
#define GET_VARIABLE_NAME(Variable) (#Variable)

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
	ImGui::SliderFloat("Camera Angle in Y axis", &gv->current_dimension.y, 0.0f, 360.0f);
	ImGui::VSliderFloat("Camera Angle in X axis", ImVec2(25, 160),&gv->current_dimension.x, 0.0f, 90.0f);
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
	gv->float_uniforms["cameraSpeed"] = 0.05f;
	//gv->vec4_uniforms["Backgound_Color"] = glm::vec4(140.0f / 255.0f, 200.0f / 255.0f, 1.0f, 1.0f);
	gv->vec4_uniforms["Backgound_Color"] = glm::vec4(1.0f,1.0f, 1.0f, 1.0f);

	// Point Light
	GraphicsBase* point_light = new GraphicsLight();
	point_light->Init_Shaders(gv->test_vs, gv->test_fs);
	point_light->Load_Model(gv->fish_model);
	point_light->vec3_uniforms["light_position"] = glm::vec3(0.0f, 100.0f, 0.0f);
	point_light->vec4_uniforms["light_color"] = glm::vec4(1.0f, 1.0f, 100.0f / 255.0f, 1.0f);
	gv->graphics.push_back(point_light);
}

void Display()
{
	auto gv = Global_Variables::Instance();
	glClearColor(gv->vec4_uniforms["Backgound_Color"].x, gv->vec4_uniforms["Backgound_Color"].y,
	             gv->vec4_uniforms["Backgound_Color"].z, gv->vec4_uniforms["Backgound_Color"].a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto g : gv->graphics)
	{
		g->Update_Uniforms();
		g->Draw();
	}

	for (auto g : gv->billboards)
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
		? gv->vec3_uniforms["cameraPos"].z += 2 * gv->float_uniforms["cameraSpeed"]
		: gv->vec3_uniforms["cameraPos"].z -= 2 * gv->float_uniforms["cameraSpeed"];
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
		gv->vec3_uniforms["cameraPos"].y += gv->float_uniforms["cameraSpeed"];
		std::cout << "cameraPos : " << gv->vec3_uniforms["cameraPos"].x << "," << gv->vec3_uniforms["cameraPos"].z << "," <<
			gv->vec3_uniforms["cameraPos"].z << std::endl;
		break;

	case 's':
	case 'S':
		gv->vec3_uniforms["cameraPos"].y -= gv->float_uniforms["cameraSpeed"];
		std::cout << "cameraPos : " << gv->vec3_uniforms["cameraPos"].x << "," << gv->vec3_uniforms["cameraPos"].z << "," <<
			gv->vec3_uniforms["cameraPos"].z << std::endl;
		break;

	case 'a':
	case 'A':
		gv->vec3_uniforms["cameraPos"] -= glm::normalize(glm::cross(gv->vec3_uniforms["cameraFront"],
		                                                            gv->vec3_uniforms["cameraUp"])) *
			gv->float_uniforms["cameraSpeed"];
		std::cout << "cameraPos : " << gv->vec3_uniforms["cameraPos"].x << "," << gv->vec3_uniforms["cameraPos"].y << "," <<
			gv->vec3_uniforms["cameraPos"].z << std::endl;
		break;

	case 'd':
	case 'D':
		gv->vec3_uniforms["cameraPos"] += glm::normalize(glm::cross(gv->vec3_uniforms["cameraFront"],
		                                                            gv->vec3_uniforms["cameraUp"])) *
			gv->float_uniforms["cameraSpeed"];
		std::cout << "cameraPos : " << gv->vec3_uniforms["cameraPos"].x << "," << gv->vec3_uniforms["cameraPos"].y << "," <<
			gv->vec3_uniforms["cameraPos"].z << std::endl;
		break;

	case 'z':
	case 'Z':
		break;

	default:
		break;
	}
}
