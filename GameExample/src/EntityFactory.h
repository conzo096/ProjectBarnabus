#pragma once
#include "GameEngine/Entity.h"
#include "GameEngine/BarnabusGameEngine.h"
#include "GameEngine/ArcBallCamera.h"
#include "GameEngine/AnimatedModel.h"
#include "GameEngine/Terrain.h"
#include "GameEngine/Movement.h"
#include "GameEngine/ArcBallCamera.h"
#include "GameEngine/FreeCamera.h"
#include "GameEngine/PhysicsContainer.h"
#include <memory>

namespace EntityFactory
{
	static void CreatePlayer(Entity* player, glm::vec3 position, IShader* shader, Terrain* terrain)
	{
		player->SetName("player");

		player->AddComponent(std::make_unique<Movement>(terrain));
		std::string fileName("res\\Models\\AnimatedModels\\Player.fbx");
		auto animatedModelComponent = std::make_unique<AnimatedModel>(fileName);
		animatedModelComponent->SetShader(shader);
		animatedModelComponent->InitModel();
		animatedModelComponent->SetAnimation("walk");

		auto physicsComponent = std::make_unique<Physics::PhysicsContainer>(true);
		physicsComponent->AddBoundingVolumes(animatedModelComponent->GetRootNode());
		physicsComponent->SetShader(BarnabusGameEngine::Get().GetShader("red"));
		physicsComponent->InitMeshes();

		player->AddComponent(std::move(animatedModelComponent));
		player->AddComponent(std::move(physicsComponent));

		player->SetPosition(position);
	}

	static void CreateSphere(Entity* sphere, glm::vec3 position, IShader* shader)
	{
		sphere->SetName("sun");

		std::string fileName("res\\Models\\Sphere.fbx");
		auto modelComponent = std::make_unique<Model>(fileName);
		modelComponent->SetShader(shader);
		modelComponent->InitModel();
		sphere->AddComponent(std::move(modelComponent));
		
		sphere->SetPosition(position);
	}

	static void CreateTerrain(Entity* terrain, IShader* shader)
	{
		terrain->SetName("terrain");
		auto fileName = "res\\Textures\\HeightMap.png";
		auto terrainComponent = std::make_unique<Terrain>(fileName, Terrain::TerrainType::Image);
		terrainComponent->SetShader(shader);
		terrainComponent->InitModel();
		terrain->AddComponent(std::move(terrainComponent));

		terrain->SetPosition(glm::vec3(-50, 0, -50));
	}

	static void CreateBuilding(Entity* building, glm::vec3 position, IShader* shader)
	{
		std::string fileName("res\\Models\\Building.fbx");
		auto modelComponent = std::make_unique<Model>(fileName);
		modelComponent->SetShader(shader);
		modelComponent->InitModel();

		auto physicsComponent = std::make_unique<Physics::PhysicsContainer>(false);
		physicsComponent->AddBoundingVolumes(modelComponent->GetRootNode());
		physicsComponent->SetShader(BarnabusGameEngine::Get().GetShader("red"));
		physicsComponent->InitMeshes();

		building->AddComponent(std::move(modelComponent));
		building->AddComponent(std::move(physicsComponent));

		building->SetPosition(position);
	}

	static void CreateCamera(Entity* camera)
	{
		camera->SetName("camera");
		auto cameraComponent = std::make_unique<ArcBallCamera>();
		camera->AddComponent(std::move(cameraComponent));
	}

	static void CreateBuilderCamera(Entity* camera)
	{
		camera->SetName("builderCamera");

		auto cameraComponent = std::make_unique<FreeCamera>(70.0f);
		cameraComponent->SetPosition(glm::vec3(0, 100, 0));
		cameraComponent->Rotate(0, -1.5708);
		camera->AddComponent(std::move(cameraComponent));

		camera->SetActive(false);
	}
}