#include "IchenLib/IchenGlut.h"
#include "Common.h"
#include "Global_Variables.h"

// Variables
int windowID;
int windowWidth;
int windowHeight;

void InitDefaultGlutEnvironment(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(5, 5);
	glutInitWindowSize(1024, 1024);
	windowID = glutCreateWindow("Ichen");
	PrintGlInfo();
}

void GlutMainLoop()
{
	glutMainLoop();
	glutDestroyWindow(windowID);
}

// Only register those required by imgui
void RegisterImGuiCallbacks()
{
	glutKeyboardUpFunc(KeyboardUpEvents);
	glutSpecialFunc(SpecialKeyboardEvents);
	glutMouseFunc(MouseEvents);
	glutMotionFunc(MotionEvents);
	glutPassiveMotionFunc(PassiveMotionEvents);
	glutReshapeFunc(Reshape);
}

void PrintGlInfo()
{
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

int GetCurrentWindowWidth()
{
	return windowWidth;
}

int GetCurrentWindowHeight()
{
	return windowHeight;
}

void KeyboardUpEvents(unsigned char key, int x, int y)
{
	ImGui_ImplGlut_KeyUpCallback(key);
}

void SpecialKeyboardEvents(int k, int x, int y)
{
	ImGui_ImplGlut_PassiveMouseMotionCallback(x, y);
}

void MouseEvents(int button, int state, int x, int y)
{
	ImGui_ImplGlut_MouseButtonCallback(button,state);
}

void MotionEvents(int x, int y)
{
	ImGui_ImplGlut_MouseMotionCallback(x, y);
}

void PassiveMotionEvents(int x, int y)
{
	ImGui_ImplGlut_MouseMotionCallback(x, y);
}

void Reshape(int w,int h)
{
	windowWidth = w;
	windowHeight = h;
	auto gv = Global_Variables::Instance();
	gv->float_uniforms["aspect"] = static_cast<float>(GetCurrentWindowWidth()) / static_cast<float>(GetCurrentWindowHeight());

	glViewport(0, 0, windowWidth, windowHeight);
}
