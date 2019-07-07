#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>

struct PositionKey
{
	float timeStamp;
	glm::vec3 position;
};

struct ScalingKey
{
	float timeStamp;
	glm::vec3 scale;
};

struct RotationKey
{
	float timeStamp;
	glm::quat rotation;
};

struct NodeAnim
{
	std::string name;
	std::vector<PositionKey> positionKeys;
	std::vector<ScalingKey> scalingKeys;
	std::vector<RotationKey> rotationKeys;

	std::vector<NodeAnim*> children;
};