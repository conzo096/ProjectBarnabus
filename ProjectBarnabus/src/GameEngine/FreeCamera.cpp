#include "FreeCamera.h"
#include "BarnabusGameEngine.h"

// Update free camera for this frame
void FreeCamera::Update(float deltaTime)
{
	if (!IsActive())
	{
		return;
	}

	projection = glm::perspective(fieldOfView, (float)1920 / 1080, 2.414f, 1000.0f);
	// The ratio of pixels to rotation
	float ratioWidth = fieldOfView / static_cast<float>(1920);
	float ratioHeight = (fieldOfView * (static_cast<float>(1080)
		/ static_cast<float>(1920)))
		/ static_cast<float>(1080);

	// Calculate the forward direction (spherical co-ordinates to Cartesian co-ordinates)
	glm::dvec3 temp_forward(cos(pitch) * -sin(yaw), sin(pitch), -cos(yaw) * cos(pitch));
	// Normalise forward direction
	forward = glm::normalize(temp_forward);

	// Create standard right vector and rotate it by the yaw
	right = glm::vec3(glm::eulerAngleY(yaw) * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	// Normalise right
	right = glm::normalize(right);

	// Calculate (perpendicular) up vector using cross product
	up = glm::cross(right, forward);
	// Normalise up
	up = glm::normalize(up);

	// Update position by multiplying translation elements by forward, up and right
	glm::vec3 trans = translation.x * right;
	trans += translation.y * up;
	trans += translation.z * forward;

	Transform::Move(trans);
	// Target vector is position vector plus forward
	target = GetPosition() + forward;

	// Reset the translation vector for the next frame
	translation = glm::vec3(0.0f, 0.0f, 0.0f);

	// Calculate view matrix
	view = glm::lookAt(GetPosition(), target, up);
}

void FreeCamera::Move(CameraMovement direction, float dist)
{
	switch (direction)
	{
	case FORWARD:
		translation.z += dist;
		break;
	case BACKWARD:
		translation.z -= dist;
		break;
	case LEFT:
		translation.x -= dist;
		break;
	case RIGHT:
		translation.x += dist;
		break;
	case UP:
		translation.y += dist;
		break;
	case DOWN:
		translation.y -= dist;
		break;
	}
}