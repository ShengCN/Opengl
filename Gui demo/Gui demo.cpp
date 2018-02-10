#include <windows.h>
#include <iostream>
#include <tchar.h>
#include <stdio.h>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glext.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "InitShader.h"
#include "LoadMesh.h"
#include "LoadTexture.h"
#include "imgui_impl_glut.h"
#include "../OpenGL_Projects/include/IchenLib/DebugCallback.h"

static const std::string vertex_shader("gui_demo_vs.glsl");
static const std::string fragment_shader("gui_demo_fs.glsl");

GLuint shader_program = -1;
GLuint texture_id = -1; //Texture map for fish

static const std::string mesh_name = "Amago0.obj";
static const std::string texture_name = "AmagoT.bmp";
MeshData mesh_data;

float angle = 0.0f;

// New Work
std::string currentDir;
float fragmentColorScalar = 0.0f;
float colors[4] = {0.0f,0.0f,0.0f,1.0f};
float selfRotateAngle = 0.0f;
float translateR = 3.0f;
// Check box region
bool isClear = true;
bool isDepth = true;
bool isOpened = true;
bool isRotate = true;
static bool isMeshBtnClicked = false;
static bool isTextureBtnClicked = false;
static bool selectedFile[1000];
std::vector<std::string> files;

void initFileSystem()
{
	std::cout << "\n";
	// file System
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	hFind = FindFirstFile(L".\\*", &FindFileData);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		printf("FindFirstFile failed (%d)\n", GetLastError());
		return;
	}
	else
	{
		do {
			if (!(FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
			{
				std::wstring tmp(FindFileData.cFileName);
				std::string str(tmp.begin(), tmp.end());
				files.push_back(str);
			}
		} while (FindNextFile(hFind, &FindFileData));
		FindClose(hFind);
	}
}

void animationMatrix()
{
	if (!isRotate)
		return;

	auto translateBackwardCenter = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	auto rotate = glm::rotate(selfRotateAngle--, glm::vec3(0.0f, 0.0f, 1.0f));
	auto animationMat = translateBackwardCenter*rotate;

	auto time_loc = glGetUniformLocation(shader_program, "time");
	if (time_loc != -1)
	{
		auto elapsedTime = glutGet(GLUT_ELAPSED_TIME) * 0.001f;
		std::cout << "Current Elapsed time:" << sin(elapsedTime) << std::endl;
		glUniform1f(time_loc, elapsedTime);
	}

	auto animation_loc = glGetUniformLocation(shader_program, "animationMatrix");
	if (animation_loc != -1)
	{
		glUniformMatrix4fv(animation_loc, 1, false, glm::value_ptr(animationMat));
	}
}

// Update some specific states of opengl
void updateGlState()
{
	if (isClear)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear the back buffer

	isDepth? glEnable(GL_DEPTH_TEST): glDisable(GL_DEPTH_TEST);

	glClearColor(colors[0], colors[1], colors[2], colors[3]);
}

void draw_gui()
{
   ImGui_ImplGlut_NewFrame();
   
   // Color
   ImGuiStyle& style = ImGui::GetStyle();
   ImVec4& col = style.Colors[ImGuiCol_TitleBg];
   col.x = 1.0f;
   col.y = 0.0f;
   col.z = 0.0f;
   ImVec4& colC = style.Colors[ImGuiCol_TitleBgCollapsed];
   colC.x = 1.0f;
   colC.y = 0.0f;
   colC.z = 0.0f;
   ImVec4& colA = style.Colors[ImGuiCol_TitleBgActive];
   colC.x = 1.0f;
   colC.y = 0.0f;
   colC.z = 0.0f;

   //uncomment the following line to create a slider which changes the viewing angle
   if (isOpened)
   {
	   ImGui::Begin("Yichen", &isOpened, ImGuiWindowFlags_AlwaysAutoResize);
	   ImGui::SliderFloat("View angle", &angle, -180.0f, +180.0f);
	   ImGui::Checkbox("Clear", &isClear);
	   ImGui::Checkbox("Depth", &isDepth);
	   ImGui::Checkbox("Rotate", &isRotate);
	   ImGui::ColorEdit4("Color", colors);
	   ImGui::SliderFloat("Fragment Color Darkness", &fragmentColorScalar,0.0f,2.0f);

	   ImGui::Text("Select a file and then click the confirm button to exchange your mesh");
	   // Mesh file
	   for (auto f = 0; f < files.size(); ++f)
	   {
		   ImGui::Selectable(files[f].c_str(), &selectedFile[f]); 
	   }
	   if (ImGui::Button("Mesh Confirm")) { printf("Mesh File\n"); isMeshBtnClicked ^= 1; }
	   if (isMeshBtnClicked)
	   {
		   // change mesh

	   }

	   ImGui::Text("Select a file and then click the confirm button to exchange your texture");
	   // Texture file
	   static bool selectedFile1[3] = { false, false, false };
	   ImGui::Selectable("main.c", &selectedFile1[0]);    ImGui::SameLine(300); ImGui::Text(" 2,345 bytes");
	   ImGui::Selectable("Hello.cpp", &selectedFile1[1]); ImGui::SameLine(300); ImGui::Text("12,345 bytes");
	   ImGui::Selectable("Hello.h", &selectedFile1[2]);   ImGui::SameLine(300); ImGui::Text(" 2,345 bytes");

	   if (ImGui::Button("Texture Confirm")) { printf("Texture File\n"); isTextureBtnClicked ^= 1; }
	   if (isTextureBtnClicked)
	   {
		   // change texture
	   }

	   ImGui::End();
   }

   static bool show_test = false;
   ImGui::ShowTestWindow(&show_test);
   ImGui::Render();
 }

// glut display callback function.
// This function gets called every time the scene gets redisplayed 
void display()
{
	updateGlState();

	glm::mat4 M = glm::rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f))*glm::scale(glm::vec3(mesh_data.mScaleFactor));
	glm::mat4 V = glm::lookAt(glm::vec3(0.0f, 1.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 P = glm::perspective(40.0f, 1.0f, 0.1f, 100.0f);

	glUseProgram(shader_program);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	int PVM_loc = glGetUniformLocation(shader_program, "PVM");
	if (PVM_loc != -1)
	{
		glm::mat4 PVM = P*V*M;
		glUniformMatrix4fv(PVM_loc, 1, false, glm::value_ptr(PVM));
	}

	int tex_loc = glGetUniformLocation(shader_program, "texture");
	if (tex_loc != -1)
	{
		glUniform1i(tex_loc, 0); // we bound our texture to texture unit 0
	}

	animationMatrix();

	int darkness_loc = glGetUniformLocation(shader_program, "darknessScalar");
	if (darkness_loc != -1)
	{
		glUniform1f(darkness_loc, fragmentColorScalar);
	}

	glBindVertexArray(mesh_data.mVao);
	glDrawElements(GL_TRIANGLES, mesh_data.mNumIndices, GL_UNSIGNED_INT, 0);

	draw_gui();

	glutSwapBuffers();
}

void idle()
{
	glutPostRedisplay();

   const int time_ms = glutGet(GLUT_ELAPSED_TIME);
   float time_sec = 0.001f*time_ms;
   int time_loc = glGetUniformLocation(shader_program, "time");
   if (time_loc != -1)
   {
      glUniform1i(time_loc, time_sec);
   }
}

void reload_shader()
{
   GLuint new_shader = InitShader(vertex_shader.c_str(), fragment_shader.c_str());

   if(new_shader == -1) // loading failed
   {
      glClearColor(1.0f, 0.0f, 1.0f, 0.0f);
   }
   else
   {
      glClearColor(0.35f, 0.35f, 0.35f, 0.0f);

      if(shader_program != -1)
      {
         glDeleteProgram(shader_program);
      }
      shader_program = new_shader;

      if(mesh_data.mVao != -1)
      {
         BufferIndexedVerts(mesh_data);
      }
   }
}

void printGlInfo()
{
   std::cout << "Vendor: "       << glGetString(GL_VENDOR)                    << std::endl;
   std::cout << "Renderer: "     << glGetString(GL_RENDERER)                  << std::endl;
   std::cout << "Version: "      << glGetString(GL_VERSION)                   << std::endl;
   std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION)  << std::endl;
}

void initOpenGl()
{
   glewInit();
   RegisterOpenGLDebug();
   glEnable(GL_DEPTH_TEST);

   reload_shader();

   //mesh and texture to be rendered
   mesh_data = LoadMesh(mesh_name);
   texture_id = LoadTexture(texture_name);
}

// glut callbacks need to send keyboard and mouse events to imgui
void keyboard(unsigned char key, int x, int y)
{
   ImGui_ImplGlut_KeyCallback(key);
   //std::cout << "key : " << key << ", x: " << x << ", y: " << y << std::endl;

   switch(key)
   {
      case 'r':
      case 'R':
         reload_shader();     
      break;
   }
}

void keyboard_up(unsigned char key, int x, int y)
{
   ImGui_ImplGlut_KeyUpCallback(key);
}

void special_up(int key, int x, int y)
{
   ImGui_ImplGlut_SpecialUpCallback(key);
}

void passive(int x, int y)
{
   ImGui_ImplGlut_PassiveMouseMotionCallback(x,y);
}

void special(int key, int x, int y)
{
   ImGui_ImplGlut_SpecialCallback(key);
}

void motion(int x, int y)
{
   ImGui_ImplGlut_MouseMotionCallback(x, y);
}

void mouse(int button, int state, int x, int y)
{
   ImGui_ImplGlut_MouseButtonCallback(button, state);
}


int main (int argc, char **argv)
{
   //Configure initial window state

   glutInit(&argc, argv); 
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
   glutInitWindowPosition (5, 5);
   glutInitWindowSize (640, 640);
   int win = glutCreateWindow ("GUI demo");

#if _DEBUG
   glutInitContextFlags(GLUT_DEBUG);
#endif
   glutInitContextVersion(4, 3);

   printGlInfo();

   //Register callback functions with glut. 
   glutDisplayFunc(display); 
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(special);
   glutKeyboardUpFunc(keyboard_up);
   glutSpecialUpFunc(special_up);
   glutMouseFunc(mouse);
   glutMotionFunc(motion);
   glutPassiveMotionFunc(motion);
   glutIdleFunc(idle);

   initOpenGl();
   initFileSystem();
   ImGui_ImplGlut_Init(); // initialize the imgui system

   //Enter the glut event loop.
   glutMainLoop();
   glutDestroyWindow(win);
   return 0;		
}