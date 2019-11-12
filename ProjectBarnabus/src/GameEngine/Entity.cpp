#include "Entity.h"
#include <algorithm>
#include <iostream>
#include "BarnabusGameEngine.h"
using namespace std;

//############## COMPONENT ##############

Component::Component(const string &token) : token(token)
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

Entity::Entity() 
{
	active = true;
	components.clear();
}

Entity::~Entity()
{	components.clear();
	BarnabusGameEngine::Get().AddMessageLog(StringLog("Goodbye from entity " + name, StringLog::Priority::Low));
}

const string Entity::GetName() const { return name; }

void Entity::SetName(string const &name) { this->name = name; }

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
	UpdateTransforms();
	for (auto &c : components)
	{
		c.second->UpdateTransforms();
		c.second->SetTransform(GetTransform() * c.second->GetTransform());
		c.second->Update(delta);
	}
}

void Entity::Render() {
	for (auto &c : components) {
		c.second->Render();
	}
}
