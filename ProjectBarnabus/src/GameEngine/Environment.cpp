#include "Environment.h"
#include "Renderer.h"
#include "BoundingVolumes.h"

namespace
{
void CompareBoundingBoxes(BoundingVolumes::BoundingBox& lhs, BoundingVolumes::BoundingBox& rhs)
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
		// todo handle collisions
		rhs.Collision();
	}
}

void CompareBoundingVolumes(BoundingVolumes::BoundingVolumes* const lhs, BoundingVolumes::BoundingVolumes* const rhs)
{
	auto& lhsBoxes = lhs->GetBoundingBoxes();
	auto& rhsBoxes = rhs->GetBoundingBoxes();

	for (int i = 0; i < lhsBoxes.size(); i++)
	{
		BoundingVolumes::BoundingBox& lhsBox = lhsBoxes[i];
		for (int j = 0; j < rhsBoxes.size(); j++)
		{
			CompareBoundingBoxes(lhsBoxes[i], rhsBoxes[j]);
		}
	}
}

void ResolveCollisions(const std::vector<BoundingVolumes::BoundingVolumes*>& boundingVolumes)
{
	for (int i = 0; i < boundingVolumes.size(); i++)
	{
		for (int j = i+1; j < boundingVolumes.size(); j++)
		{
			CompareBoundingVolumes(boundingVolumes[i], boundingVolumes[j]);
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

	std::vector<BoundingVolumes::BoundingVolumes*> boundingVolumes;
	// Add all physics objects to list
	for (auto it = entities.begin(); it != entities.end(); ++it)
	{
		if (it->second->GetCompatibleComponent<BoundingVolumes::BoundingVolumes>())
		{
			boundingVolumes.push_back(it->second->GetCompatibleComponent<BoundingVolumes::BoundingVolumes>());
		}
	}

	ResolveCollisions(boundingVolumes);

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
