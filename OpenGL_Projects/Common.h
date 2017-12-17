#pragma once
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

#define DEBUG(x,y) std::cout<<x<<"\t"<<y<<std::endl;
#define GET_VARIABLE_NAME(Variable) (#Variable)
