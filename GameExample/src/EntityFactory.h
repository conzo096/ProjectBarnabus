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
		std::string fileName("res\\Models\\AnimatedModels\\LimitedWeights.dae");
		auto animatedModelComponent = std::make_unique<AnimatedModel>(fileName);
		animatedModelComponent->SetShader(shader);
		animatedModelComponent->InitModel();
		animatedModelComponent->SetAnimation("");
		player->AddComponent(std::move(animatedModelComponent));
		player->SetRotation(glm::vec3(180, player->GetRotation().y, player->GetRotation().z));
		player->SetPosition(position);

		return player;
	}

	static std::unique_ptr<Entity> CreateTerrain(GLShader& shader)
	{
		auto terrain = std::make_unique<Entity>();
		auto fileName = "res\\Textures\\HeightMap.png";
		auto terrainComponent = std::make_unique<Terrain>(fileName, TerrainType::Image);
		terrainComponent->LoadTerrainFromHeightMap(fileName);
		terrainComponent->SetPosition(glm::vec3(-120, 0, -120));
		terrain->SetScale(glm::vec3(4, 2, 4));
		terrainComponent->SetShader(shader);
		terrainComponent->InitModel();
		terrain->AddComponent(std::move(terrainComponent));

		return terrain;
	}
}