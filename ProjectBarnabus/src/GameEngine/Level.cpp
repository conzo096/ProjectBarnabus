#include "Level.h"
#include "BarnabusGameEngine.h"
#include "PhysicsContainer.h"

namespace BarnabusFramework
{

	bool BoundingBoxColliding(BoundingVolumes::BoundingBox& lhs, BoundingVolumes::BoundingBox& rhs)
	{
		auto lhsPos = lhs.GetMinCoordinates();
		const auto lhsWidth = lhs.GetWidth();
		const auto lhsHeight = lhs.GetHeight();
		const auto lhsLength = lhs.GetLength();

		auto rhsPos = rhs.GetMinCoordinates();
		auto rhsWidth = rhs.GetWidth();
		const auto rhsHeight = rhs.GetHeight();
		const auto rhsLength = rhs.GetLength();

		if (lhsPos.x < rhsPos.x + rhsWidth && lhsPos.x + lhsWidth > rhsPos.x &&
			lhsPos.y < rhsPos.y + rhsHeight && lhsPos.y + lhsHeight > rhsPos.y &&
			lhsPos.z < rhsPos.z + rhsLength && lhsPos.z + lhsLength > rhsPos.z)
		{
			return true;
		}

		return false;
	}

	void CompareBoundingVolumes(BoundingVolumes::BoundingVolumes* const lhsBoundingVolume, Physics::PhysicsContainer* lhsPhysics, BoundingVolumes::BoundingVolumes* const rhsBoundingVolume, Physics::PhysicsContainer* rhsPhysics)
	{
		auto& lhsBoxes = lhsBoundingVolume->GetBoundingBoxes();
		auto& rhsBoxes = rhsBoundingVolume->GetBoundingBoxes();

		for (int i = 0; i < lhsBoxes.size(); i++)
		{
			BoundingVolumes::BoundingBox& lhsBox = lhsBoxes[i];
			for (int j = 0; j < rhsBoxes.size(); j++)
			{
				if (BoundingBoxColliding(lhsBoxes[i], rhsBoxes[j]))
				{
					rhsPhysics->HandleCollision(lhsPhysics, lhsBoxes[i], rhsBoxes[j]);
					lhsPhysics->HandleCollision(rhsPhysics, rhsBoxes[j], lhsBoxes[i]);
				}
			}
		}
	}

	void ResolveCollisions(const std::vector<Physics::PhysicsContainer*>& boundingVolumes)
	{
		for (int i = 0; i < boundingVolumes.size(); i++)
		{
			auto firstBoundingBox = boundingVolumes[i]->GetBoundingVolume();
			for (int j = i+1; j < boundingVolumes.size(); j++)
			{
				auto secondBoundingBox = boundingVolumes[j]->GetBoundingVolume();
				CompareBoundingVolumes(firstBoundingBox, boundingVolumes[i], secondBoundingBox, boundingVolumes[j]);
			}
			boundingVolumes[i]->Update(0);
		}
	}

	Level::Level(std::string environmentName) : name(environmentName)
	{
	}

	Level::~Level()
	{
	}

	const std::string Level::GetName()
	{
		return name;
	}

	bool Level::AddEntity(std::string entityName, Entity* entity)
	{
		auto ret = entities.insert(std::pair<std::string, Entity* >(entityName, entity));
		return ret.second;
	}

	Entity * Level::GetEntity(std::string entityName)
	{
		auto it = entities.find(entityName);
		assert(it != entities.end());

		return it->second;
	}

	void Level::RemoveEntity(const std::string & entityName)
	{
		auto entity = GetEntity(entityName);
		assert(entity);

		entity->ClearEntity();
		entities.erase(entityName);
	}

	bool Level::AddLight(std::string name, std::unique_ptr<Light> light)
	{
		light->SetName(name);
		auto ret = lights.insert(std::pair<std::string, std::unique_ptr<Light> >(name, std::move(light)));
		return ret.second;
	}

	Light* Level::GetLight(std::string lightName)
	{
		auto it = lights.find(lightName);
		assert(it != lights.end());

		return it->second.get();
	}

	void Level::Update(float deltaTime)
	{
		std::vector<Physics::PhysicsContainer*> allPhysicsObjects;
	
		// Add all physics objects to list
		for (auto it = entities.begin(); it != entities.end(); ++it)
		{
			if (it->second->GetCompatibleComponent<Physics::PhysicsContainer>())
			{
				allPhysicsObjects.push_back(it->second->GetCompatibleComponent<Physics::PhysicsContainer>());
			}
		}

		for (auto it = entities.begin(); it != entities.end(); ++it)
		{
			it->second->Update(deltaTime);
		}

		ResolveCollisions(allPhysicsObjects);

	}

	void Level::Render(float deltaTime)
	{
		for (auto it = lights.begin(); it != lights.end(); ++it)
		{
			BarnabusGameEngine::Get().GetRenderer()->AddLight(it->second.get());
		}

		for (auto it = entities.begin(); it != entities.end(); ++it)
		{
			it->second->Render();
		}
	}
}