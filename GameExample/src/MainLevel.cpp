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

namespace BarnabusFramework
{

	MainLevel::MainLevel(std::string environmentName)
		: Level(environmentName), currentMode(PLAYING), m_mainObjects(ObjectPool::MAIN), m_spawnableBuildings(ObjectPool::BUILDINGS), m_spawnableUnits(ObjectPool::UNITS)
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

			UpdateGameUI();
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

			Entity* newEntity = GetEntityFromPool(ObjectPool::MAIN);
			EntityFactory::CreateCamera(newEntity);
			AddEntity("camera", newEntity);

			newEntity = GetEntityFromPool(ObjectPool::MAIN);
			EntityFactory::CreateBuilderCamera(newEntity);
			AddEntity("builderCamera", newEntity);

			newEntity = GetEntityFromPool(ObjectPool::MAIN);
			EntityFactory::CreateTerrain(newEntity, BarnabusGameEngine::Get().GetShader("terrain"));
			AddEntity("terrain", newEntity);

			newEntity = GetEntityFromPool(ObjectPool::MAIN);
			EntityFactory::CreatePlayer(newEntity, glm::vec3(0), BarnabusGameEngine::Get().GetShader("animation"), &GetEntity("terrain")->GetComponent<Terrain>());
			AddEntity("player", newEntity);

			newEntity = GetEntityFromPool(ObjectPool::BUILDINGS);
			EntityFactory::CreateBuilding(newEntity, glm::vec3(0), BarnabusGameEngine::Get().GetShader("red"));

			AddEntity("building", newEntity);
			GetEntity("building")->SetScale(glm::vec3(2, 2, 2));

			newEntity = GetEntityFromPool(ObjectPool::MAIN);
			EntityFactory::CreateSphere(newEntity, glm::vec3(100, 300, 100), BarnabusGameEngine::Get().GetShader("red"));

			AddEntity("sun", newEntity);
			GetEntity("sun")->SetScale(glm::vec3(10, 10, 10));

			keyCallback = [this](float deltaTime) {PlayingKeyCallback(deltaTime); };

			ui.InitGameUi();
			ui.InitaliseAllQuads();

			UpdateGameUI();
		}

		MainLevel::GameMode MainLevel::GetCurrentMode()
		{
			return currentMode;
		}

		void MainLevel::UpdateGameUI()
		{
			ui.SetEntityInfoText(m_selectedEntity);
			ui.UpdateBuildingPoolLimit(m_spawnableBuildings.GetCurrentCount(), m_spawnableBuildings.MaxObjectCount);
			ui.UpdateUnitPoolLimit(m_spawnableUnits.GetCurrentCount(), m_spawnableUnits.MaxObjectCount);

			std::string mode = "Playing";
			switch (currentMode)
			{
			case PLAYING:
				mode = "Playing";
				break;
			case BUILDING:
				mode = "Building";
				break;
			};

			ui.SetExampleText(mode);

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
				camera->Rotate(5 * deltaTime, 0);
			}
			if (glfwGetKey(BarnabusGameEngine::Get().GetWindow(), GLFW_KEY_E) == GLFW_PRESS)
			{
				camera->Rotate(5 * deltaTime, 0);
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
				if (!m_tempEntity)
				{
					auto entity = GetEntityFromPool(ObjectPool::BUILDINGS);
					if (entity != NULL)
					{
						// Position does not matter as it is updated each frame until it is placed.
						EntityFactory::CreateBuilding(entity, glm::vec3(0), BarnabusGameEngine::Get().GetShader("red"));
						AddEntity(entity->GetName(), entity);
						m_tempEntity = entity;
					}
					keyCooldown = 0;
				}
			}

			if (glfwGetKey(BarnabusGameEngine::Get().GetWindow(), GLFW_KEY_3) == GLFW_PRESS && keyCooldown > 0.3f)
			{
				// Position is height * direction against direction.

				auto builderCam = GetEntity("builderCamera")->GetCompatibleComponent<FreeCamera>();
				glm::vec3 newPosition = (ray.GetDirection() * builderCam->GetPosition().y) + ray.GetPosition();
				newPosition.y = 0;


				auto entity = GetEntityFromPool(ObjectPool::UNITS);
				if (entity != NULL)
				{
					EntityFactory::CreateUnit(entity, newPosition, BarnabusGameEngine::Get().GetShader("animation"));
					AddEntity(entity->GetName(), entity);
				}
				keyCooldown = 0;
			}

			if (glfwGetKey(BarnabusGameEngine::Get().GetWindow(), GLFW_KEY_BACKSPACE) == GLFW_PRESS && keyCooldown > 0.3f)
			{
				// If there is a selected item remove it.
				if (!m_selectedEntity.empty() && m_selectedEntity != "player")
				{
					// Update the terrain if the entity is a building (callback/ event would be a better way of handling this)
					if (m_selectedEntity.find(std::string("Building")) != std::string::npos)
					{
						GetEntity("terrain")->GetComponent<Terrain>().UpdateTerrain(*GetEntity(m_selectedEntity)->GetCompatibleComponent<Physics::PhysicsContainer>(), false);
					}

					RemoveEntity(m_selectedEntity);
					m_selectedEntity.clear();
				}

				keyCooldown = 0;
			}

			// Update ray based on camera. 
			auto builderCam = GetEntity("builderCamera")->GetCompatibleComponent<FreeCamera>();
			double winX, winY;
			glfwGetCursorPos(BarnabusGameEngine::Get().GetWindow(), &winX, &winY);
			winY = BarnabusGameEngine::Get().height - winY;

			auto near = glm::unProject(glm::vec3(winX, winY, 0.0), builderCam->GetView(), builderCam->GetProjection(),
				glm::vec4(0, 0, BarnabusGameEngine::Get().width, BarnabusGameEngine::Get().height));
			auto far = glm::unProject(glm::vec3(winX, winY, 1.0), builderCam->GetView(), builderCam->GetProjection(),
				glm::vec4(0, 0, BarnabusGameEngine::Get().width, BarnabusGameEngine::Get().height));

			ray.SetPosition(near);
			far.z -= 1.0f;
			ray.SetDirection(glm::normalize(far - near));

			// camera ray to terrain.
			glm::vec3 cameraRayPosToTerrain = (ray.GetDirection() * builderCam->GetPosition().y) + ray.GetPosition();
			cameraRayPosToTerrain = GetEntity("terrain")->GetComponent<Terrain>().GetWorldPositionFromGrid(cameraRayPosToTerrain);

			// Handle mouse actions
			auto rightClickState = glfwGetMouseButton(BarnabusGameEngine::Get().GetWindow(), GLFW_MOUSE_BUTTON_RIGHT);
			auto leftClickState = glfwGetMouseButton(BarnabusGameEngine::Get().GetWindow(), GLFW_MOUSE_BUTTON_LEFT);

			// First, handle if an entity has already been created
			if (m_tempEntity)
			{
				// Update entity position 
				m_tempEntity->SetPosition(cameraRayPosToTerrain);
				Material mat;
				mat.emissive = glm::vec4(0, 1, 0, 1);
				m_tempEntity->GetComponent<Model>().SetMaterial(mat);

				bool positionInvalid = false;
				if (!GetEntity("terrain")->GetComponent<Terrain>().IsTerrainValid(*m_tempEntity->GetCompatibleComponent<Physics::PhysicsContainer>()))
				{
					Material mat;
					mat.emissive = glm::vec4(1, 0, 0, 1);
					m_tempEntity->GetComponent<Model>().SetMaterial(mat);

					positionInvalid = true;
				}

				if (rightClickState == GLFW_PRESS)
				{
					RemoveEntity(m_tempEntity->GetName());
					m_tempEntity = NULL;
				}
				else if (leftClickState == GLFW_PRESS && !positionInvalid)
				{
					m_tempEntity->GetComponent<Model>().SetMaterial(Material());
					GetEntity("terrain")->GetComponent<Terrain>().UpdateTerrain(*m_tempEntity->GetCompatibleComponent<Physics::PhysicsContainer>(), true);
					m_tempEntity = NULL;
				}
			}
			else
			{
				// Check if we are selecting an object
				if (leftClickState == GLFW_PRESS)
				{
					m_selectedEntity.clear();
					// Check through all entities and see if any are to be selected
					for (auto const& entity : entities)
					{
						glm::vec3 poi;
						if (ray.IsCollision(*entity.second, poi))
						{
							m_selectedEntity = entity.first;
							break;
						}
					}
				}
			}
		}

		Entity* MainLevel::GetEntityFromPool(ObjectPool::ObjectPoolType poolType)
		{

			ObjectPool* pool = NULL;

			switch (poolType)
			{
			case ObjectPool::MAIN:
				pool = &m_mainObjects;
				break;
			case ObjectPool::BUILDINGS:
				pool = &m_spawnableBuildings;
				break;
			case ObjectPool::UNITS:
				pool = &m_spawnableUnits;
				break;
			default:
				assert("Not implemented");
			};

			assert(pool);
			int nextFree = pool->GetNextFreeEntity();

			if (nextFree == -1)
			{
				return NULL;
			}
			return pool->GetEntity(nextFree);
		}
	}