#pragma once
#include "GameEngine/Level.h"
#include "GameEngine/Ray.h"
#include "GameUi.h"
#include "GameEngine/ObjectPool.h"
#include <functional>

class MainLevel : public Level
{
public:
	enum GameMode {
		PLAYING,
		BUILDING
	};
public:
	MainLevel(std::string environmentName);

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

	Entity* GetEntityFromPool();
private:
	float duration = 100;
	float currentTime = 0;
	GameMode currentMode;
	float keyCooldown = 0.3;
	std::function<void(float)> keyCallback;
private:
	Ray ray;
	GameUi ui;

	ObjectPool entityPool;
};