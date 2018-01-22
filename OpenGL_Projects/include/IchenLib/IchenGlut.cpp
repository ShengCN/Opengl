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

	auto gv = Global_Variables::Instance();
	if(state == GLUT_UP)
	{
		gv->isLBtnressed = false;
		gv->isRBtnPressed = false;
		
		gv->lastMMovement = glm::vec2(0);
		return;
	}
	else  // Mouse pressed
	{
		if(button == GLUT_LEFT_BUTTON)
		{
			gv->isLBtnressed = true;
			DEBUG("Current:" + to_string(x) + ",", y);
			return;
		}
		else if(button == GLUT_RIGHT_BUTTON)
		{
			gv->isRBtnPressed = true;
			DEBUG("Current:" + to_string(x) + ",", y);
			return;
		}
	}
}

void MotionEvents(int x, int y)
{
	ImGui_ImplGlut_MouseMotionCallback(x, y);

	auto gv = Global_Variables::Instance();
	// Mouse control 
	if(gv->isLBtnressed) // Mouse left button
	{
		gv->lastMMovement = glm::vec2(x - gv->lastMousePos.x, y - gv->lastMousePos.y);
	}
	else if(gv->isRBtnPressed) // Mouse right button
	{
		if (!gv->isF_MouseMovement)
		{
			const auto x_offset = x - gv->lastMousePos.x;
			const auto y_offset = y - gv->lastMousePos.y;
			gv->current_camera->ProcessMouseMovement(-x_offset, y_offset,false);
		}
		else
		{
			gv->isF_MouseMovement = false;
		}

		gv->lastMMovement = glm::vec2(x - gv->lastMousePos.x, y - gv->lastMousePos.y);
	}
	gv->lastMousePos = glm::vec2(x, y);
}

void PassiveMotionEvents(int x, int y)
{
	ImGui_ImplGlut_MouseMotionCallback(x, y);

	auto gv = Global_Variables::Instance();
	gv->lastMousePos = glm::vec2(x, y);
}

void Reshape(int w,int h)
{
	windowWidth = w;
	windowHeight = h;
	auto gv = Global_Variables::Instance();
	gv->current_camera->aspect = static_cast<float>(GetCurrentWindowWidth()) / static_cast<float>(GetCurrentWindowHeight());

	glViewport(0, 0, windowWidth, windowHeight);
}
