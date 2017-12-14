#ifndef __INITSHADER_H__
#define __INITSHADER_H__

#include <windows.h>
#include <GL/GL.h>
#include <vector>
#include <string>

GLuint InitShader(const char* computeShaderFile);
GLuint InitShader(const char* vertexShaderFile, const char* fragmentShaderFile);
GLuint InitShader(const char* vertexShaderFile, const char* geometryShader, const char* fragmentShaderFile);
GLuint InitShader_TransformFeedback(const char* vertexShaderFile, const char* fragmentShaderFile, std::vector<std::string> varying_names);
GLuint InitShader_TransformFeedback(const char* vertexShaderFile, const char* geometryShader, const char* fragmentShaderFile, std::vector<std::string> varying_names);

#endif