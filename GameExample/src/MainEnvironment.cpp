#include "MainEnvironment.h"
#include "GameEngine/DirectionalLight.h"
#include "EntityFactory.h"
#include "ShaderFactory.h"

#include "AnimationShader.h"
#include "TerrainShader.h"
#include "BasicShader.h"

#include "VkAnimationShader.h"
#include "VkTerrainShader.h"
#include "VkBasicShader.h"

#define _USE_MATH_DEFINES
#include <math.h>
MainEnvironment::MainEnvironment(std::string environmentName) : Environment(environmentName),currentMode(PLAYING)
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
	if (currentMode == PLAYING)
	{
		BarnabusGameEngine::Get().GetRenderer()->SetCameraViewProjection(GetEntity("camera")->GetComponent<ArcBallCamera>().GetProjection() * GetEntity("camera")->GetComponent<ArcBallCamera>().GetView());
	}
	else
	{
		BarnabusGameEngine::Get().GetRenderer()->SetCameraViewProjection(GetEntity("builderCamera")->GetComponent<FreeCamera>().GetProjection() * GetEntity("builderCamera")->GetComponent<FreeCamera>().GetView());
	}

	Environment::Render(deltaTime);
}

void MainEnvironment::LoadGameContent()
{
	AddLight("test", std::make_unique<DirectionalLight>(glm::vec4(0.5081, 0.5713, 0.6446, 1)));

	if (BarnabusGameEngine::Get().GetRenderType() == IRenderer::OpenGL)
	{
		ShaderFactory::CreateShader<AnimationShader>("animation", "res\\Shaders\\BasicAnimation");
		ShaderFactory::CreateShader<TerrainShader>("terrain", "res\\Shaders\\Terrain");
		ShaderFactory::CreateShader<BasicShader>("red", "res\\Shaders\\Red");
	}
	else
	{
		ShaderFactory::CreateShader<VkAnimationShader>("animation", "res\\Shaders\\Vulkan\\VkBasicAnimation");
		ShaderFactory::CreateShader<VkTerrainShader>("terrain", "res\\Shaders\\Vulkan\\VkTerrain");
		ShaderFactory::CreateShader<VkBasicShader>("red", "res\\Shaders\\Vulkan\\VkBasic");
	}

	AddEntity("camera", EntityFactory::CreateCamera());
	AddEntity("builderCamera", EntityFactory::CreateBuilderCamera());
	AddEntity("terrain", EntityFactory::CreateTerrain(BarnabusGameEngine::Get().GetShader("terrain")));
	AddEntity("player", EntityFactory::CreatePlayer(glm::vec3(0), BarnabusGameEngine::Get().GetShader("animation"), &GetEntity("terrain")->GetComponent<Terrain>()));
	AddEntity("building", EntityFactory::CreateBuilding(glm::vec3(0), BarnabusGameEngine::Get().GetShader("red")));
	GetEntity("building")->SetScale(glm::vec3(2, 2, 2));

	AddEntity("sun", EntityFactory::CreateSphere(glm::vec3(100, 300, 100), BarnabusGameEngine::Get().GetShader("red")));
	GetEntity("sun")->SetScale(glm::vec3(10, 10, 10));

	BarnabusGameEngine::Get().SetKeyCallback([this](int key, int action) { PlayingKeyCallback(key, action); });
}

MainEnvironment::GameMode MainEnvironment::GetCurrentMode()
{
	return currentMode;
}

void MainEnvironment::PlayingKeyCallback(int key, int action)
{
	Camera* camera = GetEntity("camera")->GetCompatibleComponent<ArcBallCamera>();
	glm::vec3 up = camera->GetOrientation();
	glm::vec3 dir = glm::normalize(camera->GetTarget() - camera->GetPosition());
	dir.y = 0;
	glm::vec3 left = glm::normalize(glm::cross(up, dir));
	left.y = 0;

	float speed = 0.05 * 10;

	glm::vec3 movement(0);
	if (key ==  GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		movement += dir * speed;
	}
	if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		movement += -dir * speed;
	}
	if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		movement += left * speed;
	}
	if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		movement += -left * speed;
	}
	GetEntity("player")->GetComponent<Movement>().SetMovement(movement);

	// Check if camera is to be switched.
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		BarnabusGameEngine::Get().SetKeyCallback([this](int key, int action) { BuildingKeyCallback(key, action); });
		currentMode = BUILDING;
		GetEntity("camera")->SetActive(false);
		GetEntity("builderCamera")->SetActive(true);
	}
}

void MainEnvironment::BuildingKeyCallback(int key, int action)
{
	FreeCamera* camera = GetEntity("builderCamera")->GetCompatibleComponent<FreeCamera>();
	if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		camera->Move(CameraMovement::FORWARD, 0.1);
	}
	if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		camera->Move(CameraMovement::BACKWARD, 0.1);
	}
	if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		camera->Move(CameraMovement::LEFT, 0.1);
	}
	if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		camera->Move(CameraMovement::RIGHT, 0.1);
	}

	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		BarnabusGameEngine::Get().SetKeyCallback([this](int key, int action) { PlayingKeyCallback(key, action); });
		currentMode = PLAYING;
		GetEntity("camera")->SetActive(true);
		GetEntity("builderCamera")->SetActive(false);
	}
}