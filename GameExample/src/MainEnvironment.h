#pragma once
#include "GameEngine/Environment.h"
#include <functional>

class MainEnvironment : public Environment
{
public:
	enum GameMode {
		PLAYING,
		BUILDING
	};
public:
	MainEnvironment(std::string environmentName);

public:
	void Update(float deltaTime) override;
	void Render(float deltaTime) override;
public:
	void LoadGameContent();
public:
	GameMode GetCurrentMode();

private:
	void PlayingKeyCallback(float deltaTime);
	void BuildingKeyCallback(float deltaTime);
private:
	float duration = 100;
	float currentTime = 0;
	GameMode currentMode;
	float keyCooldown = 0.3;
	std::function<void(float)> keyCallback;
};