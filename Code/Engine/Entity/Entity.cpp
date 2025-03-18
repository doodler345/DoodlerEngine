#include "Entity.h"

Entity::~Entity()
{
	DebugPrint("Destroyed Entity " + m_name + " (id: " + std::to_string(m_id) + ")", TextColor::Yellow, DebugChannel::Entity, __FILE__, __LINE__);
}

void Entity::EntityUpdate(float deltaTime)
{
	if (m_parent != nullptr)
	{
		sf::Vector2u m_parentScreenPositionDelta = sf::Vector2u(m_parent->GetScreenPosition() - m_parentLastScreenPosition);
		std::cout << m_parentScreenPositionDelta.x << " " << m_parentScreenPositionDelta.y << std::endl;
		m_transform.translate(sf::Vector2f(m_parentScreenPositionDelta));
		m_parentLastScreenPosition = m_parent->GetScreenPosition();
	}

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

void Entity::SetParent(Entity* parent)
{
	m_parent = parent;
	m_parentLastScreenPosition = m_parent->GetScreenPosition();
}

void Entity::OnInputRecieved(sf::Vector2f direction)
{
	m_transform.translate(direction);
}

sf::Vector2u Entity::GetScreenPosition()
{
	const float* matrix = GetTransform().getMatrix();
	return sf::Vector2u(matrix[12], matrix[13]);
}

EntityComponent::~EntityComponent()
{
	DebugPrint("EntityComponent destroyed", TextColor::Yellow, DebugChannel::EntityComponent, __FILE__, __LINE__);
}
