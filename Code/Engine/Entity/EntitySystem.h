#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include "Entity.h"

class EntitySystem
{
public:
	Entity* RegisterEntity(std::shared_ptr<Entity> entity, std::string m_name);
	void DestroyEntity(Entity* entity);
	void Update(float deltaTime);
private:
	std::vector<std::shared_ptr<Entity>> m_entities;
	int m_idCounter = 0;
};