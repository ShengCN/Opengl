#pragma once
#include "Common.h"
// Glut Framework Functions
void InitDefaultGlutEnvironment(int argc, char** argv);
void GlutMainLoop();
void RegisterImGuiCallbacks();

// Functions that used for glut callbacks
void KeyboardUpEvents(unsigned char key, int x, int y);
void SpecialKeyboardEvents(int k, int x, int y);
void MouseEvents(int button, int state, int x, int y);
void MotionEvents(int x, int y);
void PassiveMotionEvents(int x, int y);
void Idle();
void Reshape(int w, int h);
void PrintGlInfo();
int GetCurrentWindowWidth();
int GetCurrentWindowHeight();

