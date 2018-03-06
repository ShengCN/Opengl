#include "GraphicsParticleSystem.h"
#include "GLCommon.h"
#include "IchenLib/Utilities.h"

const int num_particles = 256*256*256;
GraphicsParticleSystem::GraphicsParticleSystem(): Write_Index(1), Read_Index(0)
{
}


GraphicsParticleSystem::~GraphicsParticleSystem()
{
}

void GraphicsParticleSystem::Draw()
{
	auto gv = Global_Variables::Instance();
	const GLint pos_loc = 0;
	const GLint vel_loc = 1;
	const GLint age_loc = 2;

	auto P = gv->current_camera->GetP();
	auto V = gv->current_camera->GetV();
	auto M = glm::rotate(Degree2Radian(gv->float_uniforms["angle"]),glm::vec3(0.0,1.0,0.0));
	auto PVM = P*V*M;
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "PVM"), 1, false, glm::value_ptr(PVM));
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "V"), 1, false, glm::value_ptr(V));
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, tfo[Write_Index]);

	glDepthMask(GL_FALSE);
	glBindVertexArray(vao[Read_Index]);
	glBeginTransformFeedback(GL_POINTS);
	// glDrawArrays(GL_POINTS, 0, num_particles);
	glDrawArraysInstanced(GL_POINTS, 0, 1, num_particles);
	glEndTransformFeedback();

	glDepthMask(GL_TRUE);
	Read_Index = 1 - Read_Index;
	Write_Index = 1 - Write_Index;
}

void GraphicsParticleSystem::Draw_Shader_Uniforms()
{
}

void GraphicsParticleSystem::Reload()
{
	const char *vars[] = { "pos_out", "vel_out", "age_out" };
	if (shader_program != -1)
	{
		glDeleteProgram(shader_program);
	}
	Init_Shaders_TransformFeedback(m_vs_file, m_fs_file, vars, 3);
	Init_Buffers();
}

void GraphicsParticleSystem::Init_Buffers()
{
	glGenTransformFeedbacks(2, tfo);
	glGenVertexArrays(2, vao);
	glGenBuffers(2, vbo);

	float* zeros = new float[7 * num_particles];
	const GLint pos_loc = 0;
	const GLint vel_loc = 1;
	const GLint age_loc = 2;

	// Write Index
	glBindVertexArray(vao[Write_Index]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[Write_Index]);
	glBufferData(GL_ARRAY_BUFFER, 7 * num_particles * sizeof(float), zeros, GL_DYNAMIC_COPY);

	glEnableVertexAttribArray(pos_loc);
	glVertexAttribPointer(pos_loc, 3, GL_FLOAT, false, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(vel_loc);
	glVertexAttribPointer(vel_loc, 3, GL_FLOAT, false, 0, BUFFER_OFFSET(num_particles * sizeof(glm::vec3)));

	glEnableVertexAttribArray(age_loc);
	glVertexAttribPointer(age_loc, 1, GL_FLOAT, false, 0, BUFFER_OFFSET(2 * num_particles * sizeof(glm::vec3)));

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, tfo[Write_Index]);
	glBindBufferRange(GL_TRANSFORM_FEEDBACK_BUFFER, pos_loc, vbo[Write_Index], 0, num_particles * 3 * sizeof(float));
	glBindBufferRange(GL_TRANSFORM_FEEDBACK_BUFFER, vel_loc, vbo[Write_Index], num_particles * 3 * sizeof(float), num_particles * 3 * sizeof(float));
	glBindBufferRange(GL_TRANSFORM_FEEDBACK_BUFFER, age_loc, vbo[Write_Index], 2 * num_particles * 3 * sizeof(float), num_particles * sizeof(float));
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);

	// Read Index
	glBindVertexArray(vao[Read_Index]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[Read_Index]);
	glBufferData(GL_ARRAY_BUFFER, 7 * num_particles * sizeof(float), zeros, GL_DYNAMIC_COPY);

	glEnableVertexAttribArray(pos_loc);
	glVertexAttribPointer(pos_loc, 3, GL_FLOAT, false, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(vel_loc);
	glVertexAttribPointer(vel_loc, 3, GL_FLOAT, false, 0, BUFFER_OFFSET(num_particles * sizeof(glm::vec3)));

	glEnableVertexAttribArray(age_loc);
	glVertexAttribPointer(age_loc, 1, GL_FLOAT, false, 0, BUFFER_OFFSET(2 * num_particles * sizeof(glm::vec3)));
	
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, tfo[Read_Index]);
	glBindBufferRange(GL_TRANSFORM_FEEDBACK_BUFFER, pos_loc, vbo[Read_Index], 0, num_particles * 3 * sizeof(float));
	glBindBufferRange(GL_TRANSFORM_FEEDBACK_BUFFER, vel_loc, vbo[Read_Index], num_particles * 3 * sizeof(float), num_particles * 3 * sizeof(float));
	glBindBufferRange(GL_TRANSFORM_FEEDBACK_BUFFER, age_loc, vbo[Read_Index], 2 * num_particles * 3 * sizeof(float), num_particles * sizeof(float));
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);

	// InitQuad(backVao, backVbo, backEbo);
	delete zeros;
}

void GraphicsParticleSystem::BufferManage()
{
}

void GraphicsParticleSystem::ReleaseBuffers()
{
}
