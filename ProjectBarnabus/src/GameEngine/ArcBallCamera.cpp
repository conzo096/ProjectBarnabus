#include "ArcBallCamera.h"
#include "BarnabusGameEngine.h"

void ArcBallCamera::Update(double deltaTime)
{
	glfwSetInputMode(BarnabusGameEngine::Get().GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	fieldOfView = 70;
	projection = glm::perspective(fieldOfView, (float)1920 / 1080, 2.414f, 1000.0f);
	// The ratio of pixels to rotation
	double ratioWidth = fieldOfView / static_cast<float>(1920);
	double ratioHeight = (fieldOfView * (static_cast<float>(1080)
		/ static_cast<float>(1920)))
		/ static_cast<float>(1080);

	double currentX, currentY;

	// Get current cursor position
	glfwGetCursorPos(BarnabusGameEngine::Get().GetWindow(), &currentX, &currentY);

	// Calculate delta of cursor positions from last frame
	double deltaX = currentX - cursorX;
	double deltaY = currentY - cursorY;

	// Multiply deltas by ratios to get change in orientation
	deltaX *= ratioWidth;
	deltaY *= ratioHeight;

	int state = glfwGetMouseButton(BarnabusGameEngine::Get().GetWindow(), GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS)
	{
		Rotate(deltaX, deltaY);
	}

	glm::dquat rotation(glm::dvec3(yAxisRotation, xAxisRotation, 0));
	glm::dvec3 newPosition = target + (rotation * glm::dvec3(0, 0, distanceFromTarget));
	up = rotation * glm::dvec3(0, 1, 0);
	SetPosition(newPosition);

	// Calculate the view matrix
	view = glm::lookAt(GetPosition(), target, up);

	glfwGetCursorPos(BarnabusGameEngine::Get().GetWindow(), &cursorX, &cursorY);
}

// Rotates the camera around the target
void ArcBallCamera::Rotate(float delta_X, float delta_Y)
{
	xAxisRotation += delta_X * 0.5;
	yAxisRotation += delta_Y * 0.5;
}