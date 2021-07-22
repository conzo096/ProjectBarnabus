#include "MainLevel.h"
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
MainLevel::MainLevel(std::string environmentName) : Level(environmentName),currentMode(PLAYING)
{
	ray.SetDirection(glm::vec3(0, -1, 0));
	ray.SetLength(150);
}

void MainLevel::Update(float deltaTime)
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

	// Updates all the components.
	Level::Update(deltaTime);

	if (currentMode == MainLevel::PLAYING)
	{
		ui.SetExampleText("PLAYING");
	}
	else if (currentMode == BUILDING)
	{
		ui.SetExampleText("BUILDING");

		auto builderCam = GetEntity("builderCamera")->GetCompatibleComponent<FreeCamera>();

		double xpos, ypos;
		glfwGetCursorPos(BarnabusGameEngine::Get().GetWindow(), &xpos, &ypos);

		double winX = xpos;
		double winY = 1080 - ypos;
	
		auto near = glm::unProject(glm::vec3(winX, winY, 0.0), builderCam->GetView(), builderCam->GetProjection(),
			glm::vec4(0, 0, 1920,1080));
		auto far = glm::unProject(glm::vec3(winX, winY, 1.0), builderCam->GetView(), builderCam->GetProjection(),
			glm::vec4(0, 0, 1920, 1080));

		ray.SetPosition(near);
		far.z -= 1.0f;
		ray.SetDirection(glm::normalize(far - near));

		int newState = glfwGetMouseButton(BarnabusGameEngine::Get().GetWindow(), GLFW_MOUSE_BUTTON_LEFT);
		if (newState == GLFW_RELEASE && m_oldMouseState == GLFW_PRESS)
		{
			for (auto const& entity : entities)
			{
				glm::vec3 poi;
				if (ray.IsCollision(*entity.second, poi))
				{
					ui.SetEntityInfoText(entity.second->GetName());
					m_selectedEntity = entity.second;
					return;
				}
			}
			if(!m_selectedEntity)
				ui.SetEntityInfoText("");
		}
		if (newState == GLFW_PRESS && m_oldMouseState == GLFW_PRESS && m_selectedEntity) // Dragging actions.
		{
			// Move entity
			auto builderCam = GetEntity("builderCamera")->GetCompatibleComponent<FreeCamera>();
			glm::vec3 newPosition = (ray.GetDirection() * builderCam->GetPosition().y) + ray.GetPosition();
			newPosition.y = 0;
			m_selectedEntity->SetPosition(newPosition);
		}
		if (newState == GLFW_RELEASE && m_oldMouseState == GLFW_RELEASE && m_selectedEntity)
		{
			ui.SetEntityInfoText("");
			m_selectedEntity = NULL;
		}
	

		m_oldMouseState = newState;

	}
}

void MainLevel::Render(float deltaTime)
{
	if (currentMode == PLAYING)
	{
		BarnabusGameEngine::Get().GetRenderer()->SetCameraViewProjection(GetEntity("camera")->GetComponent<ArcBallCamera>().GetProjection() * GetEntity("camera")->GetComponent<ArcBallCamera>().GetView());
	}
	else
	{
		BarnabusGameEngine::Get().GetRenderer()->SetCameraViewProjection(GetEntity("builderCamera")->GetComponent<FreeCamera>().GetProjection() * GetEntity("builderCamera")->GetComponent<FreeCamera>().GetView());
	}

	ui.Draw();
	Level::Render(deltaTime);
}

void MainLevel::LoadGameContent()
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

	Entity* newEntity = GetEntityFromPool();
	EntityFactory::CreateCamera(newEntity);
	AddEntity("camera", newEntity );

	newEntity = GetEntityFromPool();
	EntityFactory::CreateBuilderCamera(newEntity);
	AddEntity("builderCamera", newEntity);

	newEntity = GetEntityFromPool();
	EntityFactory::CreateTerrain(newEntity, BarnabusGameEngine::Get().GetShader("terrain"));
	AddEntity("terrain", newEntity);

	newEntity = GetEntityFromPool();
	EntityFactory::CreatePlayer(newEntity, glm::vec3(0), BarnabusGameEngine::Get().GetShader("animation"), &GetEntity("terrain")->GetComponent<Terrain>());
	AddEntity("player", newEntity);

	newEntity = GetEntityFromPool();
	EntityFactory::CreateBuilding(newEntity, glm::vec3(0), BarnabusGameEngine::Get().GetShader("red"));

	AddEntity("building", newEntity);
	GetEntity("building")->SetScale(glm::vec3(2, 2, 2));

	newEntity = GetEntityFromPool();
	EntityFactory::CreateSphere(newEntity, glm::vec3(100, 300, 100), BarnabusGameEngine::Get().GetShader("red"));

	AddEntity("sun", newEntity);
	GetEntity("sun")->SetScale(glm::vec3(10, 10, 10));

	keyCallback = [this](float deltaTime) {PlayingKeyCallback(deltaTime); };

	ui.InitGameUi();
	ui.InitaliseAllQuads();
}

MainLevel::GameMode MainLevel::GetCurrentMode()
{
	return currentMode;
}

void MainLevel::PlayingKeyCallback(float deltaTime)
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

void MainLevel::BuildingKeyCallback(float deltaTime)
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

	if (glfwGetKey(BarnabusGameEngine::Get().GetWindow(), GLFW_KEY_2) == GLFW_PRESS && keyCooldown > 0.3f)
	{
		// Position is height * direction against direction.

		auto builderCam = GetEntity("builderCamera")->GetCompatibleComponent<FreeCamera>();
		glm::vec3 newPosition = (ray.GetDirection() * builderCam->GetPosition().y ) + ray.GetPosition();
		newPosition.y = 0;


		auto entity = GetEntityFromPool();
		EntityFactory::CreateBuilding(entity, newPosition, BarnabusGameEngine::Get().GetShader("red"));
		AddEntity(entity->GetName(),entity );

		keyCooldown = 0;
	}
}

Entity * MainLevel::GetEntityFromPool()
{
	int nextFree = entityPool.GetNextFreeEntity();
	// If nextFree is a valid entity
	assert(nextFree != -1);
	return entityPool.GetEntity(nextFree);
}
