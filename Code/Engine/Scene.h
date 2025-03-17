#pragma once

#include <string>

#include "../Engine/Debug.h"
#include "Entity/EntityFactoryManager.h"

using KEY = sf::Keyboard::Key;

class Scene
{
public:
	virtual ~Scene();
	virtual void Init() = 0;
	std::string GetName();

	template <typename EntityType>
	EntityType* InstantiateEntity(std::string factoryName, std::string m_name = "");
	
	void DestroyEntity(Entity* entity);

protected:

	std::string m_name;
	std::vector<Entity*> m_loadedEntities;
};


#define Instantiate(type, m_name) InstantiateEntity<##type>(#type, #m_name)

template <typename EntityType>
inline EntityType* Scene::InstantiateEntity(std::string factoryName, std::string m_name)
{
	EntityType* instance = dynamic_cast<EntityType*>(EntityFactoryManager::GetInstance()->CreateEntity(factoryName, m_name));
	m_loadedEntities.push_back(instance);
	return instance;
}
