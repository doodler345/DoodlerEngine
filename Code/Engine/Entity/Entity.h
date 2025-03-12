#pragma once

#include <iostream>
#include <memory>
#include <assert.h>
#include "SFML/Graphics.hpp"
#include "../Engine.h"
#include "../Debug.h"


class Entity;

class EntityComponent
{
public:
	~EntityComponent();

	virtual void Update(float deltaTime) {};
	virtual void ShutDown() = 0;

	Entity* GetOwner() { return m_ownerEntity; }

	bool m_allowMultiple = false;
protected:
	Entity* m_ownerEntity = nullptr;
};


class Entity
{
public:

	~Entity();

	virtual void EntityInit() = 0;
	virtual void EntityUpdate(float deltaTime); // default: calls Update() + Updates all Components

	void Destroy(); 

	void SetID(int _id) { m_id = _id; }
	int GetID() { return m_id; }

	void SetName(std::string name){ m_name = name; }
	std::string GetName(){ return m_name; }

	template<typename T>
	bool AddComponent(std::shared_ptr<T> component);
	template<typename T>
	void RemoveComponent();
	template<typename T>
	T* GetComponent();
	void OnInputRecieved(sf::Vector2f direction);

	sf::Vector2u GetScreenPosition();
	sf::Transform& GetTransform() { return m_transform; }
	

protected:
	virtual void Update(float deltaTime) {};
	virtual void DestroyDerived() = 0; 

	std::string m_name;
	int m_id = -1;
	sf::Transform m_transform;

private:
	std::vector<std::shared_ptr<EntityComponent>> entityComponents;
};

template<typename T>
inline bool Entity::AddComponent(std::shared_ptr<T> newComponent)
{
	dynamic_cast<EntityComponent*>(newComponent.get()); // check if EntityComponent

	std::string type = typeid(T).name();
	if (!dynamic_cast<EntityComponent*>(newComponent.get())->m_allowMultiple)
	{
		for (int i = 0; i < entityComponents.size(); i++)
		{
			if (typeid(T) == typeid(*entityComponents[i].get()))
			{
				DebugPrint("Component " + type + " already exists!", TextColor::Red, DebugChannel::EntityComponent, __FILE__, __LINE__, 1);
				return false;
			}
		}
	}

	DebugPrint("Added new Component " + type + " to Entity ", TextColor::Green, DebugChannel::EntityComponent, __FILE__, __LINE__);

	entityComponents.push_back(newComponent);
	return true;
}

template<typename T>
inline void Entity::RemoveComponent()
{
	T* test;
	dynamic_cast<EntityComponent*>(test); //***** this checks if it's a EntityComponent but its ugly af
	
	std::string type = typeid(T).m_name();
	for (int i = 0; i < entityComponents.size(); i++)
	{
		if (typeid(T) == typeid(*entityComponents[i].get()))
		{
			entityComponents[i]->ShutDown();
			entityComponents.erase(entityComponents.begin() + i);
			DebugPrint("Component " + type + " of Entity " + std::to_string(m_id) + " removed!", TextColor::LightRed, DebugChannel::EntityComponent, __FILE__, __LINE__);
			break;
		}
	}
}

template<typename T>
inline T* Entity::GetComponent()
{
	T* test;
	dynamic_cast<EntityComponent*>(test); //***** this checks if it's a EntityComponent but its ugly af

	std::string type = typeid(T).name();
	for (int i = 0; i < entityComponents.size(); i++)
	{
		if (typeid(T) == typeid(*entityComponents[i].get()))
		{
			DebugPrint("Component " + type + " of Entity " + std::to_string(m_id) + " found!", TextColor::LightYellow, DebugChannel::EntityComponent, __FILE__, __LINE__);
			return dynamic_cast<T*>(entityComponents[i].get()); //***** did this just work accidentially or is this a proove solution?
		}
	}

	DebugPrint("Component " + type + " of Entity " + std::to_string(m_id) + " not found!", TextColor::Red, DebugChannel::EntityComponent, __FILE__, __LINE__, 1);
	return nullptr;
}
