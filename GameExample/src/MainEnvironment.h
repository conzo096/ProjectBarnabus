#pragma once
#include "GameEngine/Environment.h"
class MainEnvironment : public Environment
{
public:
	MainEnvironment(std::string environmentName);

public:
	void Update(float deltaTime) override;
public:
	void LoadGameContent();
};