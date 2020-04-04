#pragma once
#include "Light.h"
#include <map>
#include <memory>
class Environment
{
public:
	Environment();
	~Environment();

	bool AddLight(std::string name, std::unique_ptr<Light> light);
	Light* GetLight(std::string lightName);

private:
	std::map<std::string, std::unique_ptr<Light>> lights;
};

