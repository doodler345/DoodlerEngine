#include "CircleComponent.h"

#include "../../Engine/Engine.h"
#include "../../Engine/RenderSystem.h"

CircleComponent::CircleComponent(Entity* owner)
{
	std::string type = typeid(this).name();
	m_name = type;

	m_ownerEntity = owner;
	m_allowMultiple = true;

	m_drawable = std::make_unique<sf::CircleShape>(64);
	m_drawable->setFillColor(sf::Color::Green);


	RenderSystem& renderSystem = Engine::GetInstance()->GetRenderSystem();
	renderSystem.AddShape(m_drawable.get(), m_ownerEntity);
}

void CircleComponent::SetRadius(float radius)
{
	m_drawable->setRadius(radius);
	Center();
}

void CircleComponent::Center()
{
	float radius = m_drawable->getRadius();
	m_drawable->setOrigin(sf::Vector2f(radius, radius));
}

void CircleComponent::ShutDown()
{
	Engine::GetInstance()->GetRenderSystem().RemoveShape(m_ownerEntity);
}
