#pragma once
#include "GameEngine/Entity.h"
#include "GameEngine/BarnabusGameEngine.h"
#include "GameEngine/ArcBallCamera.h"
#include "GameEngine/AnimatedModel.h"
#include "GameEngine/Renderer.h"
#include "GameEngine/Terrain.h"
#include "GameEngine/Movement.h"
#include "GameEngine/ArcBallCamera.h"
#include "GameEngine/BoundingVolumes.h"
#include <memory>

namespace EntityFactory{

	static std::unique_ptr<Entity> CreatePlayer(glm::vec3 position, GLShader& shader, Terrain* terrain, Camera* camera)
	{
		auto player = std::make_unique<Entity>();

		player->AddComponent(std::make_unique<Movement>(terrain, camera));
		std::string fileName("res\\Models\\AnimatedModels\\Player.fbx");
		auto animatedModelComponent = std::make_unique<AnimatedModel>(fileName);
		animatedModelComponent->SetShader(shader);
		animatedModelComponent->InitModel();
		animatedModelComponent->SetAnimation("walk");
		player->AddComponent(std::move(animatedModelComponent));
		
		player->SetPosition(position);

		return player;
	}

	static std::unique_ptr<Entity> CreateSphere(glm::vec3 position, GLShader& shader)
	{
		auto sphere = std::make_unique<Entity>();

		std::string fileName("res\\Models\\Sphere.fbx");
		auto modelComponent = std::make_unique<Model>(fileName);
		modelComponent->SetShader(shader);
		modelComponent->InitModel();
		sphere->AddComponent(std::move(modelComponent));
		
		sphere->SetPosition(position);

		return sphere;
	}

	static std::unique_ptr<Entity> CreateTerrain(GLShader& shader)
	{
		auto terrain = std::make_unique<Entity>();
		auto fileName = "res\\Textures\\HeightMap.png";
		auto terrainComponent = std::make_unique<Terrain>(fileName, Terrain::TerrainType::Image);
		terrainComponent->SetShader(shader);
		terrainComponent->InitModel();
		terrain->AddComponent(std::move(terrainComponent));

		terrain->SetPosition(glm::vec3(-50, 0, -50));
		return terrain;
	}

	static std::unique_ptr<Entity> CreateBuilding(glm::vec3 position, GLShader& shader)
	{
		auto building = std::make_unique<Entity>();

		std::string fileName("res\\Models\\Building.fbx");
		auto modelComponent = std::make_unique<Model>(fileName);
		modelComponent->SetShader(shader);
		modelComponent->InitModel();
		
		auto boundingVolumesComponent = std::make_unique<BoundingVolumes::BoundingVolumes>();
		boundingVolumesComponent->AddBoundingVolumes(modelComponent->GetRootNode());

		boundingVolumesComponent->SetShader(*BarnabusGameEngine::Get().GetShader("red"));
		boundingVolumesComponent->InitMeshes();
				
		building->AddComponent(std::move(modelComponent));
		building->AddComponent(std::move(boundingVolumesComponent));
		
		building->SetPosition(position);

		return building;
	}

	static std::unique_ptr<Entity> CreateCamera()
	{
		auto camera = std::make_unique<Entity>();

		auto cameraComponent = std::make_unique<ArcBallCamera>();
		camera->AddComponent(std::move(cameraComponent));

		return camera;
	}
}