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

	float angle = (currentTime / 100) * 360;
	float radians = (angle * M_PI) / 180;

	auto sun = GetEntity("sun");
	glm::vec3 newSunPosition;
	newSunPosition.z = sun->GetPosition().z;
	newSunPosition.x = 100 * cos(radians);
	newSunPosition.y = 100 * sin(radians);
	
	sun->SetPosition(newSunPosition);

	auto worldLight = static_cast<DirectionalLight*>(GetLight("test"));
	worldLight->SetPosition(GetEntity("sun")->GetPosition());
	Environment::Update(deltaTime);
}

void MainEnvironment::LoadGameContent()
{
	AddLight("test", std::make_unique<DirectionalLight>(glm::vec4(0.5081, 0.5713, 0.6446, 1)));

	ShaderFactory::CreateShader<AnimationShader>("animation", "res\\Shaders\\BasicAnimation");
	ShaderFactory::CreateShader<HeightShader>("height", "res\\Shaders\\Height");

	AddEntity("terrain", EntityFactory::CreateTerrain(*BarnabusGameEngine::Get().GetShader("height")));
	AddEntity("player", EntityFactory::CreatePlayer(glm::vec3(0), *BarnabusGameEngine::Get().GetShader("animation"), &GetEntity("terrain")->GetComponent<Terrain>()));
	AddEntity("sun", EntityFactory::CreateSphere(glm::vec3(100, 300, 100), *BarnabusGameEngine::Get().GetShader("height")));
}
