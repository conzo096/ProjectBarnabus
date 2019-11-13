#pragma once
#include "Camera.h"
#include "glm\gtx\euler_angles.hpp"
class ArcBallCamera : public Camera
{
private:
	// Rotation around the x-axis of the target
	double xAxisRotation = 0.0f;
	// Rotation around the y-axis of the target
	double yAxisRotation = 0.0f;
	// Distance of the camera from the target
	float distanceFromTarget = 25.0f;
	// The current field of view
	float fieldOfView;
	// Cursor's current position
	double cursorX;
	double cursorY;
public:
	// Creates a new arc_ball_camera
	ArcBallCamera() : Camera("ArcBallCamera") { }
	// Gets the rotation of the camera around the x-axis
	double GetXAxisRotation() const { return xAxisRotation; }
	// Sets the rotation of the camera around the x-axis
	void SetXAxisRotation(double value) { xAxisRotation = value; }
	// Gets the rotation of the camera around the y-axis
	double GetYAxisRotation() const { return yAxisRotation; }
	// Sets the rotation of the camera around the y-axis
	void SetYAxisRotation(double value) { yAxisRotation = value; }
	// Gets the distance of the camera from the target
	float GetDistance() const { return distanceFromTarget; }
	// Sets the distance of the camera from the target
	void SetDistance(float value) { distanceFromTarget = value; }
	// Updates the camera
	void Update(float deltaTime) override;
	void Rotate(double delta_X, double delta_Y);
	// Move the camera relative to the target
	void Move(float magnitude) { distanceFromTarget += magnitude; }

	// Moves the target position by the given vector
	void Translate(const glm::vec3 &translation) { target += translation; }
};