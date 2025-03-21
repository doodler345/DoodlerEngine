#include "Entity.h"

Entity::~Entity()
{
	DebugPrint("Destroyed Entity " + m_name + " (id: " + std::to_string(m_id) + ")", TextColor::Yellow, DebugChannel::Entity, __FILE__, __LINE__);
}

void Entity::EntityUpdate(float deltaTime)
{
	ApplyParentTransform();

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

	DebugPrint("All Entity-Components of Entity " + std::to_string(m_id) + " destroyed", TextColor::Yellow, DebugChannel::Entity, __FILE__, __LINE__);

	DestroyDerived();
}

void Entity::SetParent(Entity* parent)
{
	m_parent = parent;
	m_parentLastScreenPosition = m_parent->GetScreenPosition();
	m_parentLastRotation = m_parent->GetTransformable().getRotation();
	m_parentLastScale = m_parent->GetTransformable().getScale();
}

sf::Vector2u Entity::GetScreenPosition()
{
	sf::Vector2f position = m_transformable.getPosition();
	return sf::Vector2u(position);
}

void Entity::ApplyParentTransform()
{
	if (m_parent == nullptr)
	{
		return;
	}

	//Rotation 
	float parentScreenRotation = m_parent->GetTransformable().getRotation();
	float parentScreenRotationDelta = parentScreenRotation - m_parentLastRotation;

	if (parentScreenRotationDelta != 0)
	{
		m_transformable.rotate(parentScreenRotationDelta);
	}
	m_parentLastRotation = parentScreenRotation;

	// Position
	sf::Vector2u parentScreenPos = m_parent->GetScreenPosition();
	sf::Vector2i parentScreenPositionDelta = sf::Vector2i(parentScreenPos) - sf::Vector2i(m_parentLastScreenPosition);

	if (parentScreenPositionDelta != sf::Vector2i(0, 0))
	{
		m_transformable.move(sf::Vector2f(parentScreenPositionDelta));
	}
	m_parentLastScreenPosition = m_parent->GetScreenPosition();

	//Scale
	sf::Vector2f parentScreenScale = m_parent->GetTransformable().getScale();
	sf::Vector2f parentScreenScaleDelta = parentScreenScale - m_parentLastScale;

	if (parentScreenScaleDelta != sf::Vector2f(0, 0))
	{
		// TODO: Instead of overwriting the scale, local-scale and parent-scale should multiply 
		// (have to find a way to get a local scale)
		m_transformable.setScale(m_parent->GetTransformable().getScale());
	}

	m_parentLastScale = parentScreenScale;
}

EntityComponent::EntityComponent()
{
	m_id = s_componentIDCounter++;
}

EntityComponent::~EntityComponent()
{
	//DebugPrint("EntityComponent " + m_name + " destroyed", TextColor::Yellow, DebugChannel::EntityComponent, __FILE__, __LINE__);
}

int EntityComponent::s_componentIDCounter = 0;

