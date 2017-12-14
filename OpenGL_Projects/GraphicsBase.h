#pragma once
#include <gl/glew.h>

class GraphicsBase
{
public:
	GraphicsBase();
	~GraphicsBase();

private:
	GLuint shader_program;
	
};

