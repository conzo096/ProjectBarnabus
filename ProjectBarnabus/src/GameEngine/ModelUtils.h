#pragma once
#include "AnimatedComponent.h"


glm::mat4 ConvertMat4Matrix(const aiMatrix4x4 &aiMat);

void LoadAnimatedFile(std::string& filePath, std::unique_ptr<AnimatedComponent>& component);
