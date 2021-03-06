#pragma once
#include "Light.h"
#include "Entity.h"
#include "IDayCycle.h"
#include <map>
#include <memory>
class Environment
{
public:
	Environment(std::string environmentName);
	~Environment();

	const std::string GetName();
	bool AddEntity(std::string name, std::unique_ptr<Entity> entity);
	Entity* GetEntity(std::string entityName);
	bool AddLight(std::string entityName, std::unique_ptr<Light> light);
	Light* GetLight(std::string lightName);

	virtual void Update(float deltaTime);
	virtual void Render(float deltaTime);

protected:
	std::string name;
	std::map<std::string, std::unique_ptr<Entity>> entities;
	std::map<std::string, std::unique_ptr<Light>> lights;
};

