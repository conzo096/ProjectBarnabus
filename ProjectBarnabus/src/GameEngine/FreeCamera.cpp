#include "FreeCamera.h"
#include "BarnabusGameEngine.h"

// Update free camera for this frame
void FreeCamera::Update(float deltaTime)
{
	glfwSetInputMode(BarnabusGameEngine::Get().GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	fieldOfView = 70;
	projection = glm::perspective(fieldOfView, (float)1920 / 1080, 2.414f, 1000.0f);
	// The ratio of pixels to rotation
	float ratioWidth = fieldOfView / static_cast<float>(1920);
	float ratioHeight = (fieldOfView * (static_cast<float>(1080)
		/ static_cast<float>(1920)))
		/ static_cast<float>(1080);

	double currentX;
	double currentY;

	// The camera's movement speed
	float moveSpeed = 25.0f;

	// Get current cursor position
	glfwGetCursorPos(BarnabusGameEngine::Get().GetWindow(), &currentX, &currentY);

	// Calculate delta of cursor positions from last frame
	double deltaX = (currentX - cursorX);
	double deltaY = (cursorY - currentY);

	// Multiply deltas by ratios to get change in orientation
	deltaX *= ratioWidth;
	deltaY *= ratioHeight;

	Rotate(deltaX*deltaTime, deltaY*deltaTime);
	if (glfwGetKey(BarnabusGameEngine::Get().GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
		Move(CameraMovement::FORWARD, deltaTime*moveSpeed);
	if (glfwGetKey(BarnabusGameEngine::Get().GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
		Move(CameraMovement::BACKWARD, deltaTime*moveSpeed);
	if (glfwGetKey(BarnabusGameEngine::Get().GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
		Move(CameraMovement::LEFT, deltaTime*moveSpeed);
	if (glfwGetKey(BarnabusGameEngine::Get().GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
		Move(CameraMovement::RIGHT, deltaTime*moveSpeed);

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

	glfwGetCursorPos(BarnabusGameEngine::Get().GetWindow(), &cursorX, &cursorY);
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