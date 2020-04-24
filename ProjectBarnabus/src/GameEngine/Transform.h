#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

class Transform
{
public:
	Transform() : scale(glm::vec3(1.0)), rotation(glm::quat()), position(glm::vec3(0)), previousPosition(glm::vec3(0)), transform(glm::mat4(1))
	{ 
		changed = true;
	}

	Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale) : Transform()
	{
		SetPosition(pos);
		SetRotation(rot);
		SetScale(scale);
		changed = true;
	}

	virtual ~Transform() {}

	void UpdateTransforms()
	{
		if (changed)
		{
			transform = glm::translate(position) * mat4_cast(rotation) * glm::scale(scale);
			changed = false;
		}
	}
	const bool GetChanged() const { return changed; }

	const glm::vec3 GetPosition() const { return position; }
	void SetPosition(const glm::vec3 &v3) { previousPosition = position;  position = v3;  changed = true; }
	void Move(const glm::vec3 &v3) { SetPosition(position + v3); changed = true; }

	const glm::quat GetRotation() const { return rotation; }

	void SetRotation(const glm::quat &q) { rotation = q;  changed = true; }
	void SetRotation(const glm::vec3 &v3) { rotation = glm::quat(v3); changed = true;  }
	void Rotate(const glm::quat &q) { SetRotation(rotation * q); changed = true; }
	
	void Rotate(const glm::vec3 &v3)
	{
		SetRotation(rotation * glm::quat(glm::radians(v3))); changed = true;
	}

	const glm::vec3 GetScale() const { return scale; }
	void SetScale(const glm::vec3& v3) { scale = v3;  changed = true; }
	void Scale(const glm::vec3 &v3) { scale *= v3; changed = true; }

	const glm::mat4 GetTransform() const { return transform; }
	void SetTransform(const glm::mat4 m4) { transform = m4; changed = true; }

	const glm::vec3 GetPreviousPosition() const { return previousPosition; }

protected:
	bool changed;
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;
	glm::mat4 transform;
	glm::vec3 previousPosition;
};