#include "EntityFactoryManager.h"

#include "../Debug.h"

EntityFactoryManager* EntityFactoryManager::m_s_instance = nullptr;

EntityFactoryManager* EntityFactoryManager::GetInstance()
{
	if (!m_s_instance)
		m_s_instance = new EntityFactoryManager();

	return m_s_instance;
}

Entity* EntityFactoryManager::CreateEntity(std::string factoryName, std::string entityName)
{
	StringToFactoryMap::iterator it = m_stringToFactoryMap.find(factoryName);
	if (it != m_stringToFactoryMap.end())
	{
		std::string m_name = entityName == "" ? factoryName : entityName;
		return it->second->CreateEntity(m_name);
	}

	DebugPrint("Couldnt find FactoryMap " + factoryName + " in list of registered EntityFactories!.", TextColor::Red, DebugChannel::Entity, __FILE__, __LINE__, 1);
	return nullptr;

}
