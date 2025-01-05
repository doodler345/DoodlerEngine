#include "Entity.h"

Entity::~Entity()
{
	DebugPrint("Destroyed Entity " + m_name + " (id: " + std::to_string(m_id) + ")", TextColor::Yellow, DebugChannel::Entity, __FILE__, __LINE__);
}

void Entity::EntityUpdate(float deltaTime)
{
	Update(deltaTime);

	for (int i = 0; i < entityComponents.size(); i++)
	{
		entityComponents[i]->Update(deltaTime);
	}
}

void Entity::Destroy()
{
	for (int i = 0; i < entityComponents.size(); i++)
	{
		entityComponents[i]->ShutDown();
	}
	entityComponents.clear();

	DebugPrint("All Entity-Components of Entity " + std::to_string(m_id) + " destroyed", TextColor::Yellow, DebugChannel::Entity, __FILE__, __LINE__);

	DestroyDerived();
}

void Entity::OnInputRecieved(sf::Vector2f direction)
{
	m_transform.translate(direction);
}

sf::Vector2i Entity::GetScreenPosition()
{
	const float* matrix = GetTransform().getMatrix();
	return sf::Vector2i(matrix[12], matrix[13]);
}

EntityComponent::~EntityComponent()
{
	DebugPrint("EntityComponent destroyed", TextColor::Yellow, DebugChannel::EntityComponent, __FILE__, __LINE__);
}
