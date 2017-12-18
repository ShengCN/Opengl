#include "GraphicsSphere.h"
#include "IchenLib/OpenGLHelpers.h"
#include "Global_Variables.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>


GraphicsSphere::GraphicsSphere()
{
}


GraphicsSphere::~GraphicsSphere()
{
}

void GraphicsSphere::Draw(glm::mat4 P, glm::mat4 V, glm::mat4 M)
{
}

void GraphicsSphere::Reload()
{
}

void GraphicsSphere::Draw_Shader_Uniforms()
{
}

void GraphicsSphere::BufferManage()
{
}

void GraphicsSphere::ReleaseBuffers()
{
}

void GraphicsSphere::Init_Buffers()
{

}

void GraphicsSphere::Draw()
{
	glUseProgram(shader_program);
	auto gv = Global_Variables::Instance();

	// PVM
	glm::mat4 T = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 M = T * glm::scale(glm::vec3(mesh_data.mScaleFactor));
	M = glm::rotate(float_uniforms["angle"], glm::vec3(0.0f, 1.0f, 0.0f)) * M;
	glm::mat4 V = glm::lookAt(gv->vec3_uniforms["cameraPos"], gv->vec3_uniforms["cameraPos"] + glm::vec3(0.0f, 0.0f, -0.1f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 P = glm::perspective(40.0f, gv->float_uniforms["aspect"], 0.1f, 100.0f);

	// variables
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "PVM"), 1, false, glm::value_ptr(P*V*M));
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "P"), 1, false, glm::value_ptr(P));

	glBindVertexArray(mesh_data.mVao);
	glDrawElements(GL_TRIANGLES, mesh_data.mNumIndices, GL_UNSIGNED_INT, nullptr);
	
	glBindVertexArray(0);
}

void GraphicsSphere::Reload()
{
}

void GraphicsSphere::Draw_Shader_Uniforms()
{
}

void GraphicsSphere::BufferManage()
{
}

void GraphicsSphere::ReleaseBuffers()
{
}
