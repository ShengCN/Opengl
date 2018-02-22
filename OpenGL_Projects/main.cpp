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

#define DEBUG(x,y) std::cout<<x<<"\t"<<y<<std::endl;
// #define DEBUG_REGISTER
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
	ImGui::SliderFloat("Angle", &gv->float_uniforms["angle"], 0.0f, 360.0f);			

#ifdef AUTO_GENERATE
	int i = 0;
	for (auto g : gv->graphics)
	{
	 	g->Generate_ImGui("test" + std::to_string(i++));
	}
#endif

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
	glEnable(GL_PROGRAM_POINT_SIZE); //allows us to set point size in vertex shader
	glEnable(GL_LINE_SMOOTH);

	ImGui_ImplGlut_Init();
	Init_Global();
}


void Init_Global()
{
	auto gv = Global_Variables::Instance();
	gv->current_camera->Position *= 1.0;
	gv->isImguiOpen = true;
	gv->vec3_uniforms["cameraPos"] = glm::vec3(0.0f, -0.2f, 3.0f);
	gv->vec3_uniforms["cameraFront"] = glm::vec3(0.0f, 0.0f, -1.0f);
	gv->vec3_uniforms["cameraUp"] = glm::vec3(0.0f, 1.0f, 0.0f);
	gv->vec3_uniforms["Billboard_Pos"] = glm::vec3(0.0f);
	gv->float_uniforms["cameraSpeed"] = 0.5f;
	gv->vec4_uniforms["Backgound_Color"] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	gv->current_camera->aspect = gv->float_uniforms["aspect"] = static_cast<float>(GetCurrentWindowWidth()) / static_cast<float>(GetCurrentWindowHeight());

	glClearColor(gv->vec4_uniforms["Backgound_Color"].x, gv->vec4_uniforms["Backgound_Color"].y,
		gv->vec4_uniforms["Backgound_Color"].z, gv->vec4_uniforms["Backgound_Color"].a);

	GraphicsBase* particleSystem = new GraphicsParticleSystem();
	const char *vars[] = { "pos_out", "vel_out", "age_out" };
	particleSystem->Init_Shaders_TransformFeedback(gv->particle_system_vs, gv->particle_system_fs, vars,3);
	particleSystem->Init_Buffers();
	gv->graphics.push_back(particleSystem);
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
		g->Get_Shader()==-1?
			glClearColor(1.0f, 0.0f, 1.0f, 0.0f):
			glClearColor(gv->vec4_uniforms["Backgound_Color"].x, gv->vec4_uniforms["Backgound_Color"].y,
				gv->vec4_uniforms["Backgound_Color"].z, gv->vec4_uniforms["Backgound_Color"].a);
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

	case 'c':
		gv->curveFlag = !gv->curveFlag;
		break;

	case 'p':
		gv->pointsFlag = !gv->pointsFlag;
		break;

	case 'P':
		dynamic_cast<GraphicsBezier*>(gv->graphics[0])->AddControlPointC1();
		break;

	case 'a':
		dynamic_cast<GraphicsBezier*>(gv->graphics[0])->AddControlPointRandom();
		break;

	case 'z':
	case 'Z':
		dynamic_cast<GraphicsBezier*>(gv->graphics[0])->DeleteControlPoint();
		break;

	case '1':
		static_cast<GraphicsBezier*>(gv->graphics[0])->TerminalMethod(CasteljauTerminal::IsFlat);
		DEBUG("Current methods: ","Is flat? ");
		break;
	case '2':
		static_cast<GraphicsBezier*>(gv->graphics[0])->TerminalMethod(CasteljauTerminal::IsInOnePixel);
		DEBUG("Current methods: ", "Is in one pixel? ");
		break;
	case '3':
		static_cast<GraphicsBezier*>(gv->graphics[0])->TerminalMethod(CasteljauTerminal::IsPolygonSmall);
		DEBUG("Current methods: ", "Is polygon small enough? ");
		break;
	case '4':
		static_cast<GraphicsBezier*>(gv->graphics[0])->TerminalMethod(CasteljauTerminal::IsPolygonInOnePixel);
		DEBUG("Current methods: ", "Is polygon in one pixel? ");
		break;

	default:
		break;
	}
}
