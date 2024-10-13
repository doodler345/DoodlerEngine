#include "RectangleComponent.h"

#include "../../Engine/Engine.h"
#include "../../Engine/RenderSystem.h"

RectangleComponent::RectangleComponent(Entity* owner)
{
	m_ownerEntity = owner;
	m_allowMultiple = true;

	m_drawable = std::make_unique<sf::RectangleShape>(sf::Vector2f(64, 64));
	m_drawable->setFillColor(sf::Color::Green);

	RenderSystem& renderSystem = Engine::GetInstance()->GetRenderSystem();
	renderSystem.AddShape(m_drawable.get(), m_ownerEntity);
}

void RectangleComponent::Center(bool horizontal, bool vertical)
{
	auto center = m_drawable.get()->getGlobalBounds().getSize() / 2.f;
	sf::Vector2f newOrigin = m_drawable.get()->getLocalBounds().getPosition();

	if (horizontal)
	{
		newOrigin += sf::Vector2f(center.x, 0);
	}
	else if (m_isRightAligned)
	{
		newOrigin += sf::Vector2f(2 * center.x, 0);
	}
	if (vertical)
	{
		newOrigin += sf::Vector2f(0, center.y);
	}

	m_drawable.get()->setOrigin(newOrigin);
}

void RectangleComponent::ShutDown()
{
	Engine::GetInstance()->GetRenderSystem().RemoveShape(m_ownerEntity);
}
