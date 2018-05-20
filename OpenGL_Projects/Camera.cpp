#include "Camera.h"
#include "Global_Variables.h"

Camera::Camera()
{
	MovementSpeed = 1.5f;
	MouseSensitivity = 0.1f;
	Zoom = 45.0f;
	Position = glm::vec3(0.0f, 5.0f, 3.0f);
	glm::vec3 target = glm::vec3(0.0f);
	Front = glm::normalize(target - Position);
	WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	Yaw = -90.0f;
	Pitch = 0.0f;
	updateCameraVectors();
}

void Camera::ProcessKeyboard(Camera_Movement direction, int deltaTime)
{
	float velocity = MovementSpeed * deltaTime * 0.01f;

	switch (direction)
	{
	case Camera_Movement::FORWARD:
		Position += Front * velocity;
		break;
	case Camera_Movement::BACKWARD:
		Position -= Front * velocity;
		break;
	case Camera_Movement::LEFT:
		Position -= Right * velocity;
		break;
	case Camera_Movement::RIGHT:
		Position += Right * velocity;
		break;
	case Camera_Movement::Up:
		Position += Up * velocity;
		break;
	case Camera_Movement::Down:
		Position -= Up * velocity;
		break;
	default:
		break;
	}
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}

	// Update Front, Right and Up Vectors using the updated Eular angles
	updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
	if (Zoom >= 1.0f && Zoom <= 45.0f)
		Zoom -= yoffset;
	if (Zoom <= 1.0f)
		Zoom = 1.0f;
	if (Zoom >= 45.0f)
		Zoom = 45.0f;
}

void Camera::updateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	Right = glm::normalize(glm::cross(Front, WorldUp));
	// Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = glm::normalize(glm::cross(Right, Front));
}
