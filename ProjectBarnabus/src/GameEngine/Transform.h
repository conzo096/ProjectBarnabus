#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Transform
{
public:
	Transform();

	Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);

	virtual ~Transform();

	void UpdateTransforms();

	const bool GetChanged() const;

	const glm::vec3 GetPosition() const;
	void SetPosition(const glm::vec3 v3);
	void Move(const glm::vec3 v3);

	const glm::quat GetRotation() const;

	void SetRotation(const glm::quat q);
	void SetRotation(const glm::vec3 v3);
	void Rotate(const glm::quat q);
	void Rotate(const glm::vec3 v3);

	const glm::vec3 GetScale() const;
	void SetScale(const glm::vec3 v3);
	void Scale(const glm::vec3 v3);

	const glm::mat4 GetTransform() const;
	void SetTransform(const glm::mat4 m4);

	const glm::vec3 GetPreviousPosition() const;

protected:
	bool changed;
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;
	glm::mat4 transform;
	glm::vec3 previousPosition;
};