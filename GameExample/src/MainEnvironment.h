#pragma once
#include "GameEngine/Environment.h"

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
	float duration = 100;
	float currentTime = 0;
	GameMode currentMode;
	// Used to limit key calls.
	float timeElapsed = 0.5f;
};