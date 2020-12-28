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
	static std::unique_ptr<Entity> CreatePlayer(glm::vec3 position, IShader* shader, Terrain* terrain)
	{
		auto player = std::make_unique<Entity>("player");

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

		return player;
	}

	static std::unique_ptr<Entity> CreateSphere(glm::vec3 position, IShader* shader)
	{
		auto sphere = std::make_unique<Entity>("sun");

		std::string fileName("res\\Models\\Sphere.fbx");
		auto modelComponent = std::make_unique<Model>(fileName);
		modelComponent->SetShader(shader);
		modelComponent->InitModel();
		sphere->AddComponent(std::move(modelComponent));
		
		sphere->SetPosition(position);

		return sphere;
	}

	static std::unique_ptr<Entity> CreateTerrain(IShader* shader)
	{
		auto terrain = std::make_unique<Entity>("terrain");
		auto fileName = "res\\Textures\\HeightMap.png";
		auto terrainComponent = std::make_unique<Terrain>(fileName, Terrain::TerrainType::Image);
		terrainComponent->SetShader(shader);
		terrainComponent->InitModel();
		terrain->AddComponent(std::move(terrainComponent));

		terrain->SetPosition(glm::vec3(-50, 0, -50));
		return terrain;
	}

	static std::unique_ptr<Entity> CreateBuilding(glm::vec3 position, IShader* shader)
	{
		auto building = std::make_unique<Entity>("building");

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

		return building;
	}

	static std::unique_ptr<Entity> CreateCamera()
	{
		auto camera = std::make_unique<Entity>("camera");

		auto cameraComponent = std::make_unique<ArcBallCamera>();
		camera->AddComponent(std::move(cameraComponent));

		return camera;
	}

	static std::unique_ptr<Entity> CreateBuilderCamera()
	{
		auto camera = std::make_unique<Entity>("builderCamera");

		auto cameraComponent = std::make_unique<FreeCamera>(70.0f);
		camera->AddComponent(std::move(cameraComponent));

		return camera;
	}
}