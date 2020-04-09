#include "MainEnvironment.h"
#include "GameEngine/DirectionalLight.h"
#include "EntityFactory.h"
#include "ShaderFactory.h"
#include "AnimationShader.h"
#include "HeightShader.h"
#define _USE_MATH_DEFINES
#include <math.h>
MainEnvironment::MainEnvironment(std::string environmentName) : Environment(environmentName)
{
}

void MainEnvironment::Update(float deltaTime)
{
	currentTime += deltaTime;
	if (currentTime > duration)
	{
		currentTime -= duration;
	}

	float angle = (currentTime / duration) * 360;
	float radians = (angle * M_PI) / 180;

	auto sun = GetEntity("sun");
	auto terrain = GetEntity("terrain");
	const float radius = 600;

	glm::vec3 newSunPosition;
	newSunPosition.x = (terrain->GetPosition().x + radius) * cos(radians);
	newSunPosition.y = (terrain->GetPosition().y + radius) * sin(radians);
	newSunPosition.z = (terrain->GetPosition().z + radius) * cos(radians);

	sun->SetPosition(newSunPosition);

	auto worldLight = static_cast<DirectionalLight*>(GetLight("test"));
	worldLight->SetPosition(GetEntity("sun")->GetPosition());
	Environment::Update(deltaTime);
}

void MainEnvironment::Render(float deltaTime)
{
	auto sun = GetEntity("sun");

	if (sun->GetPosition().y < -10)
	{
		sun->SetActive(false);
	}
	else
	{
		sun->SetActive(true);
	}

	Environment::Render(deltaTime);
}

void MainEnvironment::LoadGameContent()
{
	AddLight("test", std::make_unique<DirectionalLight>(glm::vec4(0.5081, 0.5713, 0.6446, 1)));

	ShaderFactory::CreateShader<AnimationShader>("animation", "res\\Shaders\\BasicAnimation");
	ShaderFactory::CreateShader<HeightShader>("height", "res\\Shaders\\Height");

	AddEntity("terrain", EntityFactory::CreateTerrain(*BarnabusGameEngine::Get().GetShader("height")));
	AddEntity("player", EntityFactory::CreatePlayer(glm::vec3(0), *BarnabusGameEngine::Get().GetShader("animation"), &GetEntity("terrain")->GetComponent<Terrain>()));
	AddEntity("sun", EntityFactory::CreateSphere(glm::vec3(100, 300, 100), *BarnabusGameEngine::Get().GetShader("height")));
	GetEntity("sun")->SetScale(glm::vec3(10, 10, 10));
}
