#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>

struct PositionKey
{
	int timeStamp;
	glm::vec3 position;
};

struct ScalingKey
{
	int timeStamp;
	glm::vec3 scale;
};

struct RotationKey
{
	int timeStamp;
	glm::quat rotation;
};

struct NodeAnim
{
	std::string name;
	std::vector<PositionKey> positionKeys;
	std::vector<ScalingKey> scalingKeys;
	std::vector<RotationKey> rotationKeys;

};