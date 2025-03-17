#include "Scene.h"

#include "Entity/EntitySystem.h"

Scene::~Scene()
{
	if (!Engine::GetInstance()->GetRenderWindow().isOpen()) return; //avoid issues after closing game

	DebugPrint("Closing Scene " + m_name, TextColor::Blue, DebugChannel::Game, __FILE__, __LINE__);

	EntitySystem& entitySystem = Engine::GetInstance()->GetEntitySystem();
	for (int i = 0; i < m_loadedEntities.size(); i++)
	{
		entitySystem.DestroyEntity(m_loadedEntities[i]);
	}
	m_loadedEntities.clear();
}

std::string Scene::GetName()
{
	return m_name;
}

void Scene::DestroyEntity(Entity* entity)
{
	
	int entityID = entity->GetID();
	for (int i = 0; i < m_loadedEntities.size(); i++)
	{
		if (m_loadedEntities[i]->GetID() == entityID)
		{
			EntitySystem& entitySystem = Engine::GetInstance()->GetEntitySystem();
			entitySystem.DestroyEntity(m_loadedEntities[i]);
			m_loadedEntities.erase(m_loadedEntities.begin() + i);
			DebugPrint("Removing " + std::to_string(entityID) + " from Scene " + m_name, TextColor::Blue, DebugChannel::Game, __FILE__, __LINE__);
			return;
		}
	}

	DebugPrint("Couldnt find Entity-ID " + std::to_string(entityID) + " to remove from Scene" + m_name, TextColor::Red, DebugChannel::Game, __FILE__, __LINE__, 1);
}
