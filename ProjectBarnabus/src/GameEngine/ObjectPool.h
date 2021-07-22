// Holds a collection of entities
#pragma once
#define MAX_OBJECT_COUNT 10

#include <vector>
#include "Entity.h"

class ObjectPool
{
public:
	ObjectPool();

	int GetNextFreeEntity(); // Returns index location of next one. -1 if none are found.
	Entity* GetEntity(int index); // inuse = true
	void FreeEntity(int index); // inuse = false

	void Update(float deltaTime);
	void Render(float deltaTime);
private:
	Entity* m_entities[MAX_OBJECT_COUNT]; // todo Use pointer?
};