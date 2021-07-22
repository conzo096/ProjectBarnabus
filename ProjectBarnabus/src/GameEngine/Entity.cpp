#include "Entity.h"
#include <algorithm>
#include <iostream>
#include "BarnabusGameEngine.h"
using namespace std;

//############## COMPONENT ##############

Component::Component(const string &token) : Transform(), token(token)
{
	entity = nullptr;
	active = true;
	BarnabusGameEngine::Get().AddMessageLog(StringLog("Creating component " + token, StringLog::Priority::Low));
}

Component::~Component()
{
	BarnabusGameEngine::Get().AddMessageLog(StringLog("Goodbye from component " + token, StringLog::Priority::Low));
	entity = nullptr;
	active = false;
	token.clear();
}

bool Component::IsActive() { return active; }

void Component::SetActive(bool b) { active = b; }

void Component::SetParent(Entity* p) { entity = p; }

Entity* Component::GetParent() const { return entity; }


//############## ENTITY ##############

Entity::Entity(const std::string & entityName) : name(entityName), active(true), m_inUse(false)
{
}

Entity::~Entity()
{	components.clear();
	BarnabusGameEngine::Get().AddMessageLog(StringLog("Goodbye from entity " + name, StringLog::Priority::Low));
}

void Entity::ClearEntity()
{
	components.clear();
	m_inUse = false;
	active = true; // todo fix this.
}

const string Entity::GetName() const { return name; }

void Entity::SetName(string const &name) { this->name = name; }

const std::string Entity::GetEnvironmentName() const
{
	return environmentName;
}

void Entity::SetEnvironmentName(std::string const & name)
{
	environmentName = name;
}

void Entity::SetActive(bool a)
{
	active = a; 

	for (auto &c : components)
	{
		c.second->SetActive(active);
	}
}

bool Entity::IsActive() { return active; }

void Entity::Update(const float delta)
{
	if (active)
	{

		UpdateTransforms();
		for (auto &c : components)
		{
			c.second->UpdateTransforms();
			c.second->SetTransform(GetTransform()*c.second->GetTransform());
			c.second->Update(delta);
		}
	}
}

bool Entity::InUse()
{
	return m_inUse;
}

void Entity::SetUse(bool inUse)
{
	m_inUse = inUse;
}

void Entity::Render() {
	if (active)
	{
		for (auto &c : components) {
			c.second->Render();
		}
	}
}
