#include "ObjectPool.h"

ObjectPool::ObjectPool()
{
	for (int i = 0; i < MAX_OBJECT_COUNT; i++)
	{
		m_entities[i] = new Entity( std::string("Entity ") + std::to_string(i) );
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
