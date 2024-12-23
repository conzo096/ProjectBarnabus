#pragma once
#include "Light.h"
#include "Entity.h"
#include "IDayCycle.h"
#include <map>
#include <memory>

namespace BarnabusFramework 
{
	class Level
	{
	public:
		Level(std::string environmentName);
		~Level();

		const std::string GetName();
		bool AddEntity(std::string name, Entity* entity);
		Entity* GetEntity(std::string entityName);
		void RemoveEntity(const std::string& entityName);
		bool AddLight(std::string entityName, std::unique_ptr<Light> light);
		Light* GetLight(std::string lightName);

		virtual void Update(float deltaTime);
		virtual void Render(float deltaTime);

	protected:
		std::string name;
		std::map<std::string, Entity*> entities;
		std::map<std::string, std::unique_ptr<Light>> lights;
	};
}