#pragma once
#include "Camera.h"
#include "BarnabusGameEngine.h"
#include "glm\gtx\euler_angles.hpp"

class FreeCamera : public Camera
{
private:
	// Current rotation on the x-axis
	double pitch;
	// Current rotation on the y-axis
	double yaw;
	// Camera's translation since previous frame
	glm::vec3 translation;
	// Cursor's current position
	double cursorX;
	double cursorY;
	// The current field of view
	float fieldOfView;


private:
	void Move(CameraMovement direction, float dist);

public:
	// Create free camera
	FreeCamera(float fov) : 
		pitch(0.0f), yaw(0.0f), 				   
		cursorX(0.0), cursorY(0.0),		
		fieldOfView(fov),
		Camera("FreeCamera") 
	{
		// Update cursor position
		glfwGetCursorPos(BarnabusGameEngine::Get().GetWindow(), &cursorX, &cursorY);
	}
	// Update free camera
	void Update(float deltaTime) override;

	// Get camera's current rotations
	float GetPitch() const
	{
		return GetPosition().x;
	}
	float GetYaw() const
	{
		return GetPosition().y;
	}

	// Set camera's current rotations
	void SetYaw(float value)
	{
		SetPosition(glm::vec3(GetPitch(), value, 0.0f));
	}
	void SetPitch(float value)
	{
		SetPosition(glm::vec3(value, GetYaw(), 0.0f));
	}

	// Build projection matrix
	void SetProjection(float aspect, float near, float far)
	{
		projection = glm::perspective(fieldOfView, aspect, near, far);
	}

	// Rotates the camera by the change in pitch and yaw
	void Rotate(double deltaYaw, double deltaPitch)
	{
		pitch += deltaPitch;
		yaw -= deltaYaw;
	}
};