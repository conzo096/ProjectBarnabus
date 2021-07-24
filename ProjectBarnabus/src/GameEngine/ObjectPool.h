// Holds a collection of entities
#pragma once


#include <vector>
#include "Entity.h"

class ObjectPool
{
public:
	enum ObjectPoolType {
		MAIN,
		BUILDINGS,
		UNITS,
		STATIC,
	};
	static const int MaxObjectCount = 10;

public:
	ObjectPool(ObjectPoolType poolType);

	int GetCurrentCount(); // How many are in use;
	int GetNextFreeEntity(); // Returns index location of next one. -1 if none are found.
	Entity* GetEntity(int index); // inuse = true
	void FreeEntity(int index); // inuse = false

	void Update(float deltaTime);
	void Render(float deltaTime);

private:
	Entity* m_entities[MaxObjectCount]; // todo Use pointer?
	ObjectPoolType m_poolType; // name of object pool
};