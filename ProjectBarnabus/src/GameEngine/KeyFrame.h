#pragma once
#include "Transform.h"
#include <map>
struct KeyFrame
{
	std::map<std::string, Transform> pose;
	float timestamp;

};