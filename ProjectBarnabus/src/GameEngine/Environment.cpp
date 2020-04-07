#include "Environment.h"
#include "Renderer.h"
#include "DirectionalLight.h"
Environment::Environment(std::string environmentName) : name(environmentName)
{
	// Adding a test light.

	AddLight("test", std::make_unique<DirectionalLight>(glm::vec4(0.5081, 0.5713, 0.6446, 1), glm::vec3(glm::vec3(0.6667, -0.3333, 0.6667))));
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
