#include <iostream>
#include <GL/glew.h>
#include <windows.h>
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
#include "Global_Variables.h"
#include "DrawObjects/GraphicsLight.h"
#include "IchenLib/Utilities.h"
#include "GraphicsBezier.h"
#include "GraphicsParticleSystem.h"
#include "IchenLib/DebugCallback.h"
#include "DrawObjects/GraphicsShaderToy.h"
#include "DrawObjects/GraphicsFish.h"
#include "GraphicsInstancePoints.h"
#include "GraphicsVolumeRendering.h"
#include "GraphicsScripts.h"
#include "SaveTexture2D.h"
#include "InkPaintingVis.h"

#define DEBUG(x,y) std::cout<<x<<"\t"<<y<<std::endl;
//#define DEBUG_REGISTER
#define AUTO_GENERATE

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
	ImGui::SliderFloat3("Light Position", &gv->vec3_uniforms["light_position"][0], -20.0f, 20.0f);
	ImGui::SliderFloat3("Object Position", &gv->vec3_uniforms["translate"][0], -20.0f, 20.0f);
	ImGui::ColorEdit3("Light Color", &gv->vec3_uniforms["light_color"][0]);
	ImGui::SliderFloat("Angle", &gv->float_uniforms["angle"], 0.0f, 360.0f);		
	ImGui::SliderFloat("Global time", &gv->gTime, 0.0, 300.0);

#ifdef AUTO_GENERATE
	int i = 0;
	for (auto g : gv->graphics)
	{
	 	g->Generate_ImGui("test" + std::to_string(i++));
	}
#endif
	//static bool show_test = true;
	//ImGui::ShowTestWindow(&show_test);

	ImGui::End();
	ImGui::Render();

}

void InitOpenGL()
{
	glewInit();
#ifdef DEBUG_REGISTER
	RegisterOpenGLDebug();
#endif

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	//glEnable(GL_SAMPLE_SHADING);
	// glEnable(GL_CULL_FACE);

	ImGui_ImplGlut_Init();
	Init_Global();
}


void Init_Global()
{
	auto gv = Global_Variables::Instance();
	gv->current_camera->Position *= 1.0;
	
	gv->int_uniforms["keyboard"] = 0;
	gv->float_uniforms["cameraSpeed"] = 0.5f;
	
	gv->vec3_uniforms["cameraPos"] = glm::vec3(0.0f, -0.2f, 3.0f);
	gv->vec3_uniforms["cameraFront"] = glm::vec3(0.0f, 0.0f, -1.0f);
	gv->vec3_uniforms["cameraUp"] = glm::vec3(0.0f, 1.0f, 0.0f);
	gv->vec3_uniforms["Billboard_Pos"] = glm::vec3(0.0f);
	gv->vec3_uniforms["light_position"] = glm::vec3(0.0f, 20.0f, 0.0);
	gv->vec3_uniforms["light_color"] = glm::vec3(1.0f);

	gv->vec4_uniforms["Backgound_Color"] = glm::vec4(1.0f);
	gv->current_camera->aspect = gv->float_uniforms["aspect"] = static_cast<float>(GetCurrentWindowWidth()) / static_cast<float>(GetCurrentWindowHeight());

	glClearColor(gv->vec4_uniforms["Backgound_Color"].r, gv->vec4_uniforms["Backgound_Color"].g,
		gv->vec4_uniforms["Backgound_Color"].b, gv->vec4_uniforms["Backgound_Color"].a);

	// process ink image

	GraphicsBase *inkpainting = new InkPaintingVis();
	dynamic_cast<InkPaintingVis*>(inkpainting)->InitSize(2,2,std::unordered_map<int, float>{std::pair<int, float>(0, 1.0f), std::pair<int, float>(1, 1.0f), std::pair<int, float>(2, 1.0f),std::pair<int,float>(3, 1.0f)});
	inkpainting->Init_Shaders(gv->ink_painting_vs, gv->ink_painting_fs);
	inkpainting->Init_Buffers();
	gv->graphics.push_back(inkpainting);
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

	if(gv->isImguiOpen)
		ImGui_Update();
	glutSwapBuffers();
}

void Idle()
{
	auto gv = Global_Variables::Instance();

	// Update all time variables
	const int time_ms = glutGet(GLUT_ELAPSED_TIME);
	gv->float_uniforms["time"] = 0.001f * time_ms - gv->lastTime;

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
		g->Get_Shader()==-1?
			glClearColor(1.0f, 0.0f, 1.0f, 0.0f):
			glClearColor(gv->vec4_uniforms["Backgound_Color"].x, gv->vec4_uniforms["Backgound_Color"].y,
				gv->vec4_uniforms["Backgound_Color"].z, gv->vec4_uniforms["Backgound_Color"].a);
	}
	gv->lastTime = 0.001f*glutGet(GLUT_ELAPSED_TIME);
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
		gv->current_camera_pos += glm::vec3(0.0, 0.0, 1.0);
		break;

	case 'a':
		gv->current_camera_pos -= glm::vec3(1.0, 0.0, 0.0);
		break;

	case 's':
		gv->current_camera_pos -= glm::vec3(0.0, 0.0, 1.0);
		break;

	case 'd':
		gv->current_camera_pos += glm::vec3(1.0, 0.0, 0.0);
		break;

	case 'n':
		gv->current_stat++;
		break;

	default:
		break;
	}
}
