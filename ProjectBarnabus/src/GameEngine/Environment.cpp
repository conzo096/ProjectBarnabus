#include "Environment.h"
#include "Renderer.h"
#include "PhysicsContainer.h"

namespace
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
	}
}

}

Environment::Environment(std::string environmentName) : name(environmentName)
{
}

Environment::~Environment()
{
}

const std::string Environment::GetName()
{
	return name;
}

bool Environment::AddEntity(std::string entityName, std::unique_ptr<Entity> entity)
{
	entity->SetEnvironmentName(name);
	auto ret = entities.insert(std::pair<std::string, std::unique_ptr<Entity> >(entityName, std::move(entity)));
	return ret.second;
}

Entity * Environment::GetEntity(std::string entityName)
{
	auto it = entities.find(entityName);
	assert(it != entities.end());

	return it->second.get();
}

bool Environment::AddLight(std::string name, std::unique_ptr<Light> light)
{
	light->SetName(name);
	auto ret = lights.insert(std::pair<std::string, std::unique_ptr<Light> >(name, std::move(light)));
	return ret.second;
}

Light* Environment::GetLight(std::string lightName)
{
	auto it = lights.find(lightName);
	assert(it != lights.end());

	return it->second.get();
}

void Environment::Update(float deltaTime)
{
	for (auto it = entities.begin(); it != entities.end(); ++it)
	{
		it->second->Update(deltaTime);
	}

	std::vector<Physics::PhysicsContainer*> allPhysicsObjects;
	// Add all physics objects to list
	for (auto it = entities.begin(); it != entities.end(); ++it)
	{
		if (it->second->GetCompatibleComponent<Physics::PhysicsContainer>())
		{
			allPhysicsObjects.push_back(it->second->GetCompatibleComponent<Physics::PhysicsContainer>());
		}
	}

	ResolveCollisions(allPhysicsObjects);

}

void Environment::Render(float deltaTime)
{
	for (auto it = lights.begin(); it != lights.end(); ++it)
	{
		Renderer::Get().AddLight(name, it->second.get());
	}

	for (auto it = entities.begin(); it != entities.end(); ++it)
	{
		it->second->Render();
	}
}
