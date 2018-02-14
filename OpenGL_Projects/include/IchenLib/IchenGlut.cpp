#include "IchenLib/IchenGlut.h"
#include "Common.h"
#include "Global_Variables.h"

// Variables
int windowID;
int windowWidth;
int windowHeight;

void InitDefaultGlutEnvironment(int argc, char** argv)
{
	auto gv = Global_Variables::Instance();

#if _DEBUG
	glutInitContextFlags(GLUT_DEBUG);
#endif
	glutInitContextVersion(4, 6);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(static_cast<int>(gv->windowPos.x), static_cast<int>(gv->windowPos.y));
	glutInitWindowSize(gv->width, gv->height);
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
	auto gv = Global_Variables::Instance();
	ImGui_ImplGlut_PassiveMouseMotionCallback(x, y);
	switch(k)
	{
	case GLUT_KEY_UP:
		gv->current_camera->ProcessKeyboard(Camera_Movement::FORWARD, gv->delta_time);
		gv->current_camera->Debug_Current_Pos();
		break;

	case GLUT_KEY_DOWN:
		gv->current_camera->ProcessKeyboard(Camera_Movement::BACKWARD, gv->delta_time);
		gv->current_camera->Debug_Current_Pos();
		break;

	case GLUT_KEY_LEFT:
		gv->current_camera->ProcessKeyboard(Camera_Movement::LEFT, gv->delta_time);
		gv->current_camera->Debug_Current_Pos();
		break;

	case GLUT_KEY_RIGHT:
		gv->current_camera->ProcessKeyboard(Camera_Movement::RIGHT, gv->delta_time);
		gv->current_camera->Debug_Current_Pos();
		break;
	default:
		break;
	}
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
			DEBUG("Current:" + std::to_string(x) + ",", y);
			return;
		}
		else if(button == GLUT_RIGHT_BUTTON)
		{
			gv->isRBtnPressed = true;
			DEBUG("Current:" + std::to_string(x) + ",", y);
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
	gv->mouseX = x;
	gv->mouseY = y;
}

void Reshape(int w,int h)
{
	windowWidth = w;
	windowHeight = h;
	auto gv = Global_Variables::Instance();
	gv->current_camera->aspect = static_cast<float>(GetCurrentWindowWidth()) / static_cast<float>(GetCurrentWindowHeight());
	gv->width = w;
	gv->height = h;

	glViewport(0, 0, windowWidth, windowHeight);
}
