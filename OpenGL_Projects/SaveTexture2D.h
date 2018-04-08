#pragma once
#include<string>
#include "GL/glew.h"
#include "GL/gl.h"

bool SaveTexture2D(const std::string filename, GLuint texture, const int width, const int height);