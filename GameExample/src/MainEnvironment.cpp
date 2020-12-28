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

#include <glm/matrix.hpp>
MainEnvironment::MainEnvironment(std::string environmentName) : Environment(environmentName),currentMode(PLAYING)
{
	ray.SetDirection(glm::vec3(0, -1, 0));
	ray.SetLength(150);
}

void MainEnvironment::Update(float deltaTime)
{
	// Handle input
	keyCooldown += deltaTime;
	keyCallback(deltaTime);

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

	if (currentMode == BUILDING)
	{

		auto builderCam = GetEntity("builderCamera")->GetCompatibleComponent<FreeCamera>();
		// position should be based on where the cursor on the screen - will test with center of camera for now.
		double xpos, ypos;
		glfwGetCursorPos(BarnabusGameEngine::Get().GetWindow(), &xpos, &ypos);
		glm::vec2 mouse = glm::vec2(xpos, ypos);

		double winX = (double)mouse.x;
		double winY = 1080 - (double)mouse.y;

		auto near = glm::unProject(glm::vec3(winX, winY, 0.0), builderCam->GetView(), builderCam->GetProjection(),
			glm::vec4(0, 0, 1920,1080));
		auto far = glm::unProject(glm::vec3(winX, winY, 1.0), builderCam->GetView(), builderCam->GetProjection(),
			glm::vec4(0, 0, 1920, 1080));

		ray.SetPosition(near);
		far.z -= 1.0f;
		ray.SetDirection(glm::normalize(far - near));

		glm::vec3 poi;
		ray.IsCollision( *GetEntity("player"), poi);
	}
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

	keyCallback = [this](float deltaTime) {PlayingKeyCallback(deltaTime); };
}

MainEnvironment::GameMode MainEnvironment::GetCurrentMode()
{
	return currentMode;
}

void MainEnvironment::PlayingKeyCallback(float deltaTime)
{
	Camera* camera = GetEntity("camera")->GetCompatibleComponent<ArcBallCamera>();
	glm::vec3 up = camera->GetOrientation();
	glm::vec3 dir = glm::normalize(camera->GetTarget() - camera->GetPosition());
	dir.y = 0;
	glm::vec3 left = glm::normalize(glm::cross(up, dir));
	left.y = 0;

	float speed = deltaTime * 10;

	glm::vec3 movement(0);
	if (glfwGetKey(BarnabusGameEngine::Get().GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
	{
		movement += dir * speed;
	}
	if (glfwGetKey(BarnabusGameEngine::Get().GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
	{
		movement += -dir * speed;
	}
	if (glfwGetKey(BarnabusGameEngine::Get().GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
	{
		movement += left * speed;
	}
	if (glfwGetKey(BarnabusGameEngine::Get().GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
	{
		movement += -left * speed;
	}
	GetEntity("player")->GetComponent<Movement>().SetMovement(movement);

	// Check if camera is to be switched.
	if (glfwGetKey(BarnabusGameEngine::Get().GetWindow(), GLFW_KEY_1) == GLFW_PRESS && keyCooldown > 0.3f)
	{
		keyCallback = [this](float deltaTime) {BuildingKeyCallback(deltaTime); };
		currentMode = BUILDING;
		GetEntity("camera")->SetActive(false);
		GetEntity("builderCamera")->SetActive(true);

		keyCooldown = 0;
	}
}

void MainEnvironment::BuildingKeyCallback(float deltaTime)
{
	FreeCamera* camera = GetEntity("builderCamera")->GetCompatibleComponent<FreeCamera>();
	// The camera's movement speed - Maybe better stored in class.
	float moveSpeed = 25.0f * deltaTime;

	if (glfwGetKey(BarnabusGameEngine::Get().GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
	{
		camera->Move(CameraMovement::UP, moveSpeed);
	}
	if (glfwGetKey(BarnabusGameEngine::Get().GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
	{
		camera->Move(CameraMovement::DOWN, moveSpeed);
	}
	if (glfwGetKey(BarnabusGameEngine::Get().GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
	{
		camera->Move(CameraMovement::LEFT, moveSpeed);
	}
	if (glfwGetKey(BarnabusGameEngine::Get().GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
	{
		camera->Move(CameraMovement::RIGHT, moveSpeed);
	}
	if (glfwGetKey(BarnabusGameEngine::Get().GetWindow(), GLFW_KEY_Q) == GLFW_PRESS)
	{
		camera->Rotate(5*deltaTime, 0);
	}
	if (glfwGetKey(BarnabusGameEngine::Get().GetWindow(), GLFW_KEY_E) == GLFW_PRESS)
	{
		camera->Rotate(5*deltaTime, 0);
	}

	if (glfwGetKey(BarnabusGameEngine::Get().GetWindow(), GLFW_KEY_1) == GLFW_PRESS && keyCooldown > 0.3f)
	{
		keyCallback = [this](float deltaTime) {PlayingKeyCallback(deltaTime); };
		currentMode = PLAYING;
		GetEntity("camera")->SetActive(true);
		GetEntity("builderCamera")->SetActive(false);

		keyCooldown = 0;
	}
}