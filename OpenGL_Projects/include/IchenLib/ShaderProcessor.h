#ifndef __INITSHADER_H__
#define __INITSHADER_H__

#include <windows.h>
#include <GL/GL.h>
#include <vector>
#include <string>

GLuint InitShader(const char* computeShaderFile);
GLuint InitShader(const char* vertexShaderFile, const char* fragmentShaderFile);
GLuint InitShader(const char* vertexShaderFile, const char* geometryShader, const char* fragmentShaderFile);
GLuint InitShader_TransformFeedback(const char* vertexShaderFile, const char* fragmentShaderFile, const char *vars[], unsigned int size);
GLuint InitShader_TransformFeedback(const char* vertexShaderFile, const char* geometryShader, const char* fragmentShaderFile, const char *vars[], unsigned int size);

#endif