#include "ArcBallCamera.h"
#include "BarnabusGameEngine.h"

void ArcBallCamera::Update(float deltaTime)
{
	glfwSetInputMode(BarnabusGameEngine::Get().GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	fieldOfView = 70;
	projection = glm::perspective(fieldOfView, (float)BarnabusGameEngine::Get().width / (float)BarnabusGameEngine::Get().height, 2.414f, 1000.0f);
	// The ratio of pixels to rotation
	double ratioWidth = fieldOfView / (double)BarnabusGameEngine::Get().width;
	double ratioHeight = (fieldOfView * ((double)BarnabusGameEngine::Get().height / (double)BarnabusGameEngine::Get().width))
		/ (double)BarnabusGameEngine::Get().height;

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

	glm::quat rotation(glm::vec3(yAxisRotation, xAxisRotation, 0));
	glm::vec3 newPosition = target + (rotation * glm::vec3(0, 0, distanceFromTarget));
	up = rotation * glm::vec3(0, 1, 0);
	SetPosition(newPosition);

	// Calculate the view matrix
	view = glm::lookAt(GetPosition(), target, up);

	forward = glm::normalize(target);

	glfwGetCursorPos(BarnabusGameEngine::Get().GetWindow(), &cursorX, &cursorY);
}

// Rotates the camera around the target
void ArcBallCamera::Rotate(double delta_X, double delta_Y)
{
	xAxisRotation += delta_X * 0.5;
	yAxisRotation += delta_Y * 0.5;
}