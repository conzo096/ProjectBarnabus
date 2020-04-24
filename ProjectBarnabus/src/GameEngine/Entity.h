#pragma once
#include <map>
#include <memory>
#include <string>
#include <typeindex>
#include <vector>
#include "Transform.h"

class Entity;

class Component : public Transform
{
protected:
	Entity* entity;
	bool active;
	Component() = delete;
	Component(const Component &obj) = delete;

public:
	std::string token;
	explicit Component(const std::string &token);

	virtual ~Component();
	virtual void Update(float delta) {};
	virtual void Render() {};
	bool IsActive();
	void SetActive(bool b);
	virtual void SetParent(Entity *p);
	Entity *GetParent() const;
};

class Entity : public Transform
{
protected:
	std::string name;
	std::string environmentName;
	using map = std::map<std::type_index, std::unique_ptr<Component>>;
	map components;
	bool active;
public:
	Entity();
	~Entity();

	const std::string GetName() const;
	void SetName(std::string const &name);
	const std::string GetEnvironmentName() const;
	void SetEnvironmentName(std::string const &name);
	bool IsActive();
	void SetActive(bool a);

	void Update(const float delta);
	void Render();

	template <typename T> T &GetComponent() const
	{
		map::const_iterator iter = components.find(std::type_index(typeid(T)));
		if (iter != components.end())
		{
			return *static_cast<T *>(iter->second.get());
		}
		throw;
	}

	template <typename T> void AddComponent(std::unique_ptr<T> component) noexcept
	{
		Component *a = component.get();
		a->SetParent(this);
		components[std::type_index(typeid(T))] = std::move(component);
	}

	// Will return a T component, or anything derived from a T component.
	template <typename T> T *const GetCompatibleComponent()
	{
		for (auto &c : components)
		{
			auto dd = dynamic_cast<T *>(&(*c.second));
			if (dd)
			{
				return dd;
			}
		}
		return NULL;
	}

	
};