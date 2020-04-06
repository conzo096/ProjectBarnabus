#include "Environment.h"

Environment::Environment(std::string environmentName) : name(environmentName)
{
}


Environment::~Environment()
{
}

bool Environment::AddEntity(std::string name, std::unique_ptr<Entity> entity)
{
	auto ret = entities.insert(std::pair<std::string, std::unique_ptr<Entity> >(name, std::move(entity)));
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
}

void Environment::Render(float deltaTime)
{
	for (auto it = entities.begin(); it != entities.end(); ++it)
	{
		it->second->Render();
	}
}
