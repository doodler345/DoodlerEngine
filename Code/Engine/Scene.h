#pragma once

#include <string>

#include "../Engine/Debug.h"
#include "Entity/EntityFactoryManager.h"

using KEY = sf::Keyboard::Key;

class Scene
{
public:
	~Scene();
	virtual void Init() = 0;
	virtual void Destroy() {};

protected:
	template <typename EntityType>
	EntityType* InstantiateEntity(std::string factoryName, std::string m_name = "");

	std::string m_name;
	std::vector<Entity*> m_loadedEntities;
};

//#define Instantiate(type, name) (##type*)InstantiateEntity(#type, #name)
#define Instantiate(type, m_name) InstantiateEntity<##type>(#type, #m_name)

template <typename EntityType>
inline EntityType* Scene::InstantiateEntity(std::string factoryName, std::string m_name)
{
	EntityType* instance = dynamic_cast<EntityType*>(EntityFactoryManager::GetInstance()->CreateEntity(factoryName, m_name));
	m_loadedEntities.push_back(instance);
	return instance;
}
