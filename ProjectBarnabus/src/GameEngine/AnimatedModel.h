#pragma once
#include "Model.h"
#include "Bone.h"

#include <map>

class AnimatedModel : public Model
{
public:

	AnimatedModel(const std::string& fileName);

	std::vector<Bone> bones;
	std::map<std::string, int> boneMapping;
};