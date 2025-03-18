#pragma once

#include <iostream>
#include <memory>
#include <map>

#include "Entity.h"

class EntitySystem
{
public:
	Entity* RegisterEntity(std::shared_ptr<Entity> entity, std::string m_name);
	void DestroyEntity(Entity* entity);
	void Update(float deltaTime);
private:
	std::map<int, std::shared_ptr<Entity> > m_entities;
	std::vector< std::shared_ptr<Entity> > m_entitiesToDestroy;
	int m_idCounter = 0;
};