#include "Transform.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

Transform::Transform() : scale(glm::vec3(1.0)), rotation(glm::quat()), position(glm::vec3(0)), previousPosition(glm::vec3(0)), transform(glm::mat4(1)), changed(true)
{
}

Transform::Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale) : Transform()
{
	SetPosition(pos);
	SetRotation(rot);
	SetScale(scale);
	changed = true;
}

Transform::~Transform()
{
}

void Transform::UpdateTransforms()
{
	if (changed)
	{
		transform = glm::translate(position) * mat4_cast(rotation) * glm::scale(scale);
		changed = false;
	}
}

const bool Transform::GetChanged() const
{
	return changed;
}

const glm::vec3 Transform::GetPosition() const
{
	return position;
}

void Transform::SetPosition(const glm::vec3 v3)
{
	position = v3;
	changed = true;
}

void Transform::Move(const glm::vec3 v3)
{
	previousPosition = position;
	SetPosition(position + v3);
	changed = true;
}

const glm::quat Transform::GetRotation() const
{
	return rotation;
}

void Transform::SetRotation(const glm::quat q)
{
	rotation = q; 
 changed = true;
}

void Transform::SetRotation(const glm::vec3 v3)
{
	rotation = glm::quat(v3); 
	changed = true;
}

void Transform::Rotate(const glm::quat q)
{
	SetRotation(rotation * q); changed = true;
}

void Transform::Rotate(const glm::vec3 v3)
{
	SetRotation(rotation * glm::quat(glm::radians(v3)));
	changed = true;
}

const glm::vec3 Transform::GetScale() const
{
	return scale;
}

void Transform::SetScale(const glm::vec3 v3)
{
	scale = v3;  changed = true;
}

void Transform::Scale(const glm::vec3 v3)
{
	scale *= v3;
	changed = true;
}

const glm::mat4 Transform::GetTransform() const
{
	return transform;
}

void Transform::SetTransform(const glm::mat4 m4)
{
	transform = m4;
	changed = true;
}

const glm::vec3 Transform::GetPreviousPosition() const
{
	return previousPosition;
}