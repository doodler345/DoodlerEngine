#include "EntitySystem.h"

#include <assert.h>
#include "../Debug.h"

Entity* EntitySystem::RegisterEntity(std::shared_ptr<Entity> entity, std::string m_name)
{
	entity->SetName(m_name);
	entity->SetID(m_idCounter);
	m_entities[m_idCounter] = entity;
	m_idCounter++;

	DebugPrint("Registered Entity " + m_name + " (id: " + std::to_string(entity->GetID()) + ")", TextColor::Green, DebugChannel::Entity, __FILE__, __LINE__);
	entity->EntityInit();

	return entity.get();
}

void EntitySystem::DestroyEntity(Entity* entity)
{
	if (auto search = m_entities.find(entity->GetID()); search != m_entities.end()) 
	{
		m_entitiesToDestroy.push_back(search->second);
		search->second->Destroy();
	}
	else 
	{
		DebugPrint("Can't find Entity " + entity->GetName() + " (id: " + std::to_string(entity->GetID()) + ")", TextColor::Green, DebugChannel::Entity, __FILE__, __LINE__, 1);
	}
}

void EntitySystem::Update(float deltaTime)
{
	// Has to be done like this because entities can be destroyed during EntityUpdate
	for (int i = 0; i < m_entitiesToDestroy.size(); i++)
	{	
		m_entities.erase(m_entitiesToDestroy[i]->GetID());
	}
	m_entitiesToDestroy.clear();

	for (auto& it : m_entities)
	{
		it.second->EntityUpdate(deltaTime);
	}
}
