#pragma once

#include <iostream>
#include <memory>
#include <assert.h>
#include "SFML/Graphics.hpp"
#include "../Engine.h"
#include "../Entity/EntitySystem.h"
#include "../Entity/EntityComponent.h"
#include "../Debug.h"

class Entity
{
public:

	~Entity();

	virtual void EntityInit() = 0;

	void EntityUpdate(float deltaTime); // calls ApplyParentTransform(), Update() + Updates all Components
	void Destroy(); 

	void SetID(int _id) { m_id = _id; }
	void SetName(std::string name){ m_name = name; }
	void SetParent(Entity* parent);

	int GetID() { return m_id; }
	std::string GetName(){ return m_name; }

	template<typename T>
	bool AddComponent(std::shared_ptr<T> component);
	template<typename T>
	void RemoveComponent();
	template<typename T>
	T* GetComponent();

	sf::Vector2u GetScreenPosition();
	sf::Transformable& GetTransformable() { return m_transformable; }
	

protected:
	virtual void SetActive(bool value) { }
	virtual void Update(float deltaTime) {};
	virtual void DestroyDerived() = 0; 

	std::string m_name;
	int m_id = -1;
	sf::Transformable m_transformable;

private:
	void ApplyParentTransform();

	std::vector<std::shared_ptr<EntityComponent>> entityComponents;
	Entity* m_parent = nullptr;
	sf::Vector2u m_parentLastScreenPosition;
	float m_parentLastRotation;
	sf::Vector2f m_parentLastScale;
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
				DebugPrint("Component " + type + " already exists on Entity " + m_name + "  (e_id: " + std::to_string(m_id) + ")", TextColor::Red, DebugChannel::EntityComponent, __FILE__, __LINE__, 1);
				return false;
			}
		}
	}

	DebugPrint("Added new Component " + type + " (c_id: " + std::to_string(newComponent->m_id) + ") to Entity " + m_name + "  (e_id: " + std::to_string(m_id) + ")", TextColor::Green, DebugChannel::EntityComponent, __FILE__, __LINE__);

	entityComponents.push_back(newComponent);
	return true;
}

template<typename T>
inline void Entity::RemoveComponent()
{
	T* test;
	dynamic_cast<EntityComponent*>(test); //checks if it's a EntityComponent 

	std::string type = typeid(T).name();
	for (int i = 0; i < entityComponents.size(); i++)
	{
		if (typeid(T) == typeid(*entityComponents[i].get()))
		{
			entityComponents[i]->ShutDown();
			int componentID = entityComponents[i]->m_id;
			entityComponents.erase(entityComponents.begin() + i); // TODO: if multiple components of the same type exist, only the first one will be removed 
			DebugPrint("Component " + type + " (c_id: " + std::to_string(componentID) + ") of Entity " + m_name + " (e_id: " + std::to_string(m_id) + ") removed!", TextColor::LightRed, DebugChannel::EntityComponent, __FILE__, __LINE__);
			break;
		}
	}
}

template<typename T>
inline T* Entity::GetComponent()
{
	T* test;
	dynamic_cast<EntityComponent*>(test); //checks if it's a EntityComponent 

	std::string type = typeid(T).name();
	for (int i = 0; i < entityComponents.size(); i++)
	{
		if (typeid(T) == typeid(*entityComponents[i].get()))
		{
			int componentID = entityComponents[i]->m_id;
			DebugPrint("Component " + type + " (c_id: " + std::to_string(componentID) + ") of Entity " + m_name + " (e_id: " + std::to_string(m_id) + ") found!", TextColor::LightYellow, DebugChannel::EntityComponent, __FILE__, __LINE__);
			return dynamic_cast<T*>(entityComponents[i].get());
		}
	}

	DebugPrint("Component " + type + " of Entity " + m_name + " (e_id: " + std::to_string(m_id) + ") not found!", TextColor::Red, DebugChannel::EntityComponent, __FILE__, __LINE__, 1);
	return nullptr;
}
