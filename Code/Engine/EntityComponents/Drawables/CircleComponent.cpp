#include "CircleComponent.h"

#include "../../Engine/Engine.h"
#include "../../Engine/RenderSystem.h"

CircleComponent::CircleComponent(Entity* owner, int renderLayer)
{
	std::string type = typeid(this).name();
	m_name = type;

	m_ownerEntity = owner;
	m_allowMultiple = true;
	m_renderLayer = renderLayer;

	m_drawable = std::make_unique<sf::CircleShape>(64);
	m_drawable->setFillColor(sf::Color::Green);


	RenderSystem& renderSystem = Engine::GetInstance()->GetRenderSystem();
	renderSystem.AddEntry(m_drawable.get(), this, EntryType::ShapeEntry, m_renderLayer);
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

void CircleComponent::SetVisibility(bool value)
{
	if (m_isVisible == value)
	{
		return;
	}

	m_isVisible = value;

	if (value)
	{
		m_renderSystem->AddEntry(m_drawable.get(), this, EntryType::ShapeEntry, m_renderLayer);
	}
	else
	{
		m_renderSystem->RemoveEntry(this, EntryType::ShapeEntry);
	}
}

void CircleComponent::SetRenderLayer(int layer)
{
	if (m_renderLayer == layer)
	{
		return;
	}

	m_renderLayer = layer;

	if (m_isVisible)
	{
		m_renderSystem->RemoveEntry(this, EntryType::ShapeEntry);
		m_renderSystem->AddEntry(m_drawable.get(), this, EntryType::ShapeEntry, m_renderLayer);
	}
}

void CircleComponent::ShutDown()
{
	Engine::GetInstance()->GetRenderSystem().RemoveEntry(this, EntryType::ShapeEntry);
}
