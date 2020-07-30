#include "ResourceManager.h"
#include "BarnabusGameEngine.h"
#include "AnimatedModel.h"
#include "Terrain.h"

template <typename T>
void ResourceManager::AddModel(std::string filepath)
{
	if (models.find(filepath) != models.end())
	{
		BarnabusGameEngine::Get().AddMessageLog(StringLog("Model already exists", StringLog::Priority::Critical));
		return;
	}
	
	auto model = new T(filepath);

	models.insert(std::pair<std::string, Model*>(filepath, model));
}

template void ResourceManager::AddModel<Model>(std::string filepath);
template void ResourceManager::AddModel<Terrain>(std::string filepath);
template void ResourceManager::AddModel<AnimatedModel>(std::string filepath);