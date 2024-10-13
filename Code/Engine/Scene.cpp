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
