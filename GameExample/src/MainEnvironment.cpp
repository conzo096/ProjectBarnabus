#include "MainEnvironment.h"
#include "GameEngine/DirectionalLight.h"
#include "EntityFactory.h"
#include "ShaderFactory.h"
#include "AnimationShader.h"
#include "TerrainShader.h"
#include "RedShader.h"

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


	auto camera = GetEntity("camera");
	camera->GetComponent<ArcBallCamera>().SetTarget(GetEntity("player")->GetPosition() + glm::vec3(0, 5, 0));

	Environment::Update(deltaTime);
}

void MainEnvironment::Render(float deltaTime)
{
	BarnabusGameEngine::Get().GetRenderer()->SetCameraViewProjection(GetEntity("camera")->GetComponent<ArcBallCamera>().GetProjection() * GetEntity("camera")->GetComponent<ArcBallCamera>().GetView());

	Environment::Render(deltaTime);
}

void MainEnvironment::LoadGameContent()
{
	AddLight("test", std::make_unique<DirectionalLight>(glm::vec4(0.5081, 0.5713, 0.6446, 1)));

	if (BarnabusGameEngine::Get().GetRenderType() == IRenderer::OpenGL)
	{
		ShaderFactory::CreateShader<AnimationShader>("animation", "res\\Shaders\\BasicAnimation");
		ShaderFactory::CreateShader<TerrainShader>("terrain", "res\\Shaders\\Terrain");
		ShaderFactory::CreateShader<RedShader>("red", "res\\Shaders\\Red");
	}
	else
	{
		ShaderFactory::CreateShader<AnimationShader>("animation", "res\\Shaders\\Vulkan\\VkRed");
		ShaderFactory::CreateShader<TerrainShader>("terrain", "res\\Shaders\\Vulkan\\VkRed");
		ShaderFactory::CreateShader<RedShader>("red", "res\\Shaders\\Vulkan\\VkRed");
	}

	AddEntity("camera", EntityFactory::CreateCamera());
	AddEntity("terrain", EntityFactory::CreateTerrain(BarnabusGameEngine::Get().GetShader("red")));
	AddEntity("player", EntityFactory::CreatePlayer(glm::vec3(0), BarnabusGameEngine::Get().GetShader("red"), &GetEntity("terrain")->GetComponent<Terrain>(),GetEntity("camera")->GetCompatibleComponent<ArcBallCamera>()));
	AddEntity("building", EntityFactory::CreateBuilding(glm::vec3(0), BarnabusGameEngine::Get().GetShader("red")));
	GetEntity("building")->SetScale(glm::vec3(2, 2, 2));

	AddEntity("sun", EntityFactory::CreateSphere(glm::vec3(100, 300, 100), BarnabusGameEngine::Get().GetShader("red")));
	GetEntity("sun")->SetScale(glm::vec3(10, 10, 10));
}
