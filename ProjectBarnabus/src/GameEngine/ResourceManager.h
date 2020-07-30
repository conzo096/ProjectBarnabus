#pragma once
#include "Singleton.h"
#include "Model.h"

class ResourceManager : public Singleton<ResourceManager>
{
public:
    template <typename T>
	void AddModel(std::string filepath);

private:
	std::map<std::string, Model*> models;
};