#pragma once
#include <vector>
#include "GL/glew.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

enum class Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera
{
public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	// Eular angles
	float Yaw;
	float Pitch;
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	// Aspect
	float aspect;

	Camera();

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 GetV()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	glm::mat4 GetP()
	{
		return glm::perspective(Zoom, aspect, 0.1f, 10000.0f);
	}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, int deltaTime);

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(float yoffset);

	void Debug_Current_Pos() {
		std::cout << Position.x << "\t" << Position.y << "\t"
			<< Position.z << std::endl;
	}

private:
	// Calculates the front vector from the Camera's (updated) Eular Angles
	void updateCameraVectors();
};

