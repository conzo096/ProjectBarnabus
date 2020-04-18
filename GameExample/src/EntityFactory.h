#pragma once
#include "GameEngine/Entity.h"
#include "GameEngine/BarnabusGameEngine.h"
#include "GameEngine/ArcBallCamera.h"
#include "GameEngine/AnimatedModel.h"
#include "GameEngine/Renderer.h"
#include "GameEngine/Terrain.h"
#include "GameEngine/Movement.h"
#include <memory>

namespace EntityFactory{

	static std::unique_ptr<Entity> CreatePlayer(glm::vec3 position, GLShader& shader, Terrain* terrain)
	{
		auto player = std::make_unique<Entity>();

		player->AddComponent(std::make_unique<Movement>(terrain));
		//std::string fileName("res\\Models\\AnimatedModels\\Player.fbx");
		//auto animatedModelComponent = std::make_unique<AnimatedModel>(fileName);
		//animatedModelComponent->SetShader(shader);
		//animatedModelComponent->InitModel();
		//animatedModelComponent->SetAnimation("walk");
		//player->AddComponent(std::move(animatedModelComponent));
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
		building->AddComponent(std::move(modelComponent));

		building->SetPosition(position);

		return building;
	}
}