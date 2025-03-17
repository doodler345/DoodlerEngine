#pragma once

#include <iostream>
#include <map>
#include <string>
#include <memory>
#include "../Engine.h"
#include "../Entity/Entity.h"

class EntityFactory
{
public:
	virtual Entity* CreateEntity(std::string m_name) { return nullptr; }; //***** better way to make it abstract? Normal pure virtual doesnt work.
};

template<typename EntityType>
class EntityFactory_Impl : public EntityFactory
{
public:
	Entity* CreateEntity(std::string m_name)
	{ 
		std::shared_ptr<EntityType> newEntity = std::make_shared<EntityType>();
		return Engine::GetInstance()->GetEntitySystem().RegisterEntity(newEntity, m_name);
	}
};


class EntityFactoryManager
{
public:
	static EntityFactoryManager* GetInstance();

	template<typename EntityType> 
	void Register(std::string factoryName);
	Entity* CreateEntity(std::string factoryName, std::string entityName = "");

private:

	static EntityFactoryManager* m_s_instance;
	using StringToFactoryMap = std::map<std::string, std::shared_ptr<EntityFactory>>;
	StringToFactoryMap m_stringToFactoryMap;
};

#define REGISTER(EntityType) Register<##EntityType>(#EntityType);

template<typename EntityType>
inline void EntityFactoryManager::Register(std::string m_name)
{
	std::shared_ptr <EntityFactory_Impl<EntityType>> newFactoryMap = std::make_shared<EntityFactory_Impl<EntityType>>();
	m_stringToFactoryMap[m_name] = newFactoryMap;
}
