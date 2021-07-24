#include "ObjectPool.h"

ObjectPool::ObjectPool(ObjectPoolType poolType) : m_poolType(poolType)
{
	std::string poolName;
	switch (poolType)
	{
	case MAIN:
		poolName = "Main";
		break;
	case BUILDINGS:
		poolName = "Building";
		break;
	case UNITS:
		poolName = "Unit";
		break;
	case STATIC:
		poolName = "Static";
		break;
	default:
		poolName = "Undefined";
		break;
	};

	for (int i = 0; i < MAX_OBJECT_COUNT; i++)
	{
		m_entities[i] = new Entity( poolName + std::to_string(i) );
	}
}

int ObjectPool::GetNextFreeEntity()
{
	for (int i = 0; i < MAX_OBJECT_COUNT; i++)
	{
		if (!m_entities[i]->InUse())
			return i;
	}

	return -1;
}

Entity * ObjectPool::GetEntity(int index)
{
	assert(index < MAX_OBJECT_COUNT);
	m_entities[index]->SetUse(true);
	return m_entities[index];
}

void ObjectPool::FreeEntity(int index)
{
	assert(index < MAX_OBJECT_COUNT);
	m_entities[index]->ClearEntity();
}

void ObjectPool::Update( float deltaTime )
{
	for (auto e : m_entities)
	{
		e->Update( deltaTime );
	}
}

void ObjectPool::Render(float deltaTime)
{
	for (auto e : m_entities)
	{
		e->Update(deltaTime);
	}
}
