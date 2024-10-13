#include "EntitySystem.h"

#include <assert.h>
#include "../Debug.h"

Entity* EntitySystem::RegisterEntity(std::shared_ptr<Entity> entity, std::string m_name)
{
	entity->SetName(m_name);
	entity->SetID(m_idCounter++);
	entity->EntityInit();
	m_entities.push_back(entity);

	//***** instead id, name, for better debugging
	DebugPrint("Registered Entity " + m_name + " (id: " + std::to_string(entity->GetID()) + ")", TextColor::Green, DebugChannel::Entity, __FILE__, __LINE__);

	return entity.get();
}

void EntitySystem::DestroyEntity(Entity* entity)
{
	bool success = false;

	for (int i = 0; i < m_entities.size(); i++)
	{
		if (entity->GetID() == m_entities[i]->GetID())
		{
			success = true;
			m_entities[i]->Destroy();
			m_entities.erase(m_entities.begin() + i);
			break;
		}
	}

	assert(success);
}

void EntitySystem::Update(float deltaTime)
{
	for (int i = 0; i < m_entities.size(); i++)
	{
		m_entities[i]->EntityUpdate(deltaTime);
	}
}
