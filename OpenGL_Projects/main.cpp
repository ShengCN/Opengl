#include <iostream>
#include <GL/glew.h>
#include <windows.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <stdio.h>

#include "imgui/imgui_impl_glut.h"
#include "IchenLib/IchenGlut.h"
#include "IchenLib/ShaderProcessor.h"
#include "imgui/imgui.h"
#include "IchenLib/OpenGLHelpers.h"
#include "IchenLib/LoadMesh.h"
#include "IchenLib/LoadTexture.h"
#include "Global_Variables.h"
#include "GraphicsFish.h"
#include "GraphicsBillboard.h"

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

void Init_Global()
{
	auto gv = Global_Variables::Instance();
	gv->vec3_uniforms["cameraPos"] = glm::vec3(0.0f, -0.2f, 3.0f);
	gv->vec3_uniforms["cameraFront"] = glm::vec3(0.0f, 0.0f, -1.0f);
	gv->vec3_uniforms["cameraUp"] = glm::vec3(0.0f, 1.0f, 0.0f);
	gv->vec3_uniforms["Billboard_Pos"] = glm::vec3(0.0f);
	gv->float_uniforms["cameraSpeed"] = 0.05f;
	gv->vec4_uniforms["Backgound_Color"] = glm::vec4(140.0f/255.0f, 200.0f/255.0f, 1.0f, 1.0f);

	// Graphics
	GraphicsBase *billboard = new GraphicsBillboard();
	billboard->Init_Shaders(gv->billboard_vs,gv->billboard_gs ,gv->billboard_fs);
	billboard->Init_Buffers();
	billboard->Load_Texture(gv->billBoard_texture);

	gv->graphics.push_back(billboard);
}

void ImGui_Update()
{
	auto gv = Global_Variables::Instance();

	ImGui_ImplGlut_NewFrame();
	static bool isShown = true;
	auto isBegin = ImGui::Begin("Debug", &isShown, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::ColorEdit4("Background Color", &gv->vec4_uniforms["Backgound_Color"][0]);
	ImGui::SliderFloat3("Billboard Position", &gv->vec3_uniforms["Billboard_Pos"][0], -5.0, 5.0f);
	for (auto g : gv->graphics)
	{
		g->Generate_ImGui("test");
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

	auto gv = Global_Variables::Instance();
}

void Display()
{
	auto gv = Global_Variables::Instance();
	glClearColor(gv->vec4_uniforms["Backgound_Color"].x, gv->vec4_uniforms["Backgound_Color"].y, gv->vec4_uniforms["Backgound_Color"].z, gv->vec4_uniforms["Backgound_Color"].a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for(auto g:gv->graphics)
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

	for(auto g:gv->graphics)
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
		gv->vec3_uniforms["cameraPos"] -= glm::normalize(glm::cross(gv->vec3_uniforms["cameraFront"], gv->vec3_uniforms["cameraUp"])) *
			gv->float_uniforms["cameraSpeed"];
		std::cout << "cameraPos : " << gv->vec3_uniforms["cameraPos"].x << "," << gv->vec3_uniforms["cameraPos"].y << "," <<
			gv->vec3_uniforms["cameraPos"].z << std::endl;
		break;

	case 'd':
	case 'D':
		gv->vec3_uniforms["cameraPos"] += glm::normalize(glm::cross(gv->vec3_uniforms["cameraFront"], gv->vec3_uniforms["cameraUp"])) *
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
