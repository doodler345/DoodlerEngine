#include "TextComponent.h"

#include <iostream>

#include "../../RenderSystem.h"
#include "../../Engine.h"


TextComponent::TextComponent(std::string& path, std::string& text, Entity* owner, sf::Color color, bool centerHorizontal, bool centerVertical, int renderLayer) :
	m_fontPath(path), m_textColor(color), m_centerHorizontal(centerHorizontal), m_centerVertical(centerVertical)
{
	std::string type = typeid(this).name();
	m_name = type;

	m_ownerEntity = owner;
	m_renderLayer = renderLayer;

	ResourceManager& resourceManager = Engine::GetInstance()->GetResourceManager();
	m_renderSystem = &Engine::GetInstance()->GetRenderSystem();
		
	m_font = resourceManager.RegisterFont(m_fontPath);
	m_text.setFont(m_font);
	m_text.setFillColor(m_textColor);
	m_text.setString(text);
	SetFontSize(20);
	CenterText();

	m_renderSystem->AddEntry(&m_text, this, EntryType::TextEntry, m_renderLayer);
}

void TextComponent::SetVisibility(bool value)
{
	if (m_isVisible == value)
	{
		return;
	}

	m_isVisible = value;

	if (m_isVisible)
	{
		m_renderSystem->AddEntry(&m_text, this, EntryType::TextEntry, m_renderLayer);
	}
	else
	{
		m_renderSystem->RemoveEntry(this, EntryType::TextEntry);
	}
}

void TextComponent::SetRenderLayer(int layer)
{
	if (m_renderLayer == layer)
	{
		return;
	}

	m_renderLayer = layer;
		
	if (m_isVisible)
	{
		m_renderSystem->RemoveEntry(this, EntryType::TextEntry);
		m_renderSystem->AddEntry(&m_text, this, EntryType::TextEntry, m_renderLayer);
	}
}

void TextComponent::ShutDown()
{
	Engine::GetInstance()->GetRenderSystem().RemoveEntry(this, EntryType::TextEntry);
}

void TextComponent::SetText(std::string text)
{
	m_text.setString(text);
	CenterText();
}

void TextComponent::SetTextColor(sf::Color color)
{
	m_textColor = color;
	m_text.setFillColor(m_textColor);
}

void TextComponent::SetFont(std::string& path)
{
	ResourceManager& resourceManager = Engine::GetInstance()->GetResourceManager();

	m_fontPath = path;
	m_font = resourceManager.RegisterFont(m_fontPath);
	m_text.setFont(m_font);

	CenterText();
}

void TextComponent::SetFontSize(int fontSize)
{
	m_text.setCharacterSize(fontSize);
	this->m_fontSize = fontSize;

	CenterText();
}

void TextComponent::CenterText()
{
	auto center = m_text.getGlobalBounds().getSize() / 2.f;
	sf::Vector2f newOrigin = m_text.getLocalBounds().getPosition();

	if (m_centerHorizontal)
	{
		newOrigin += sf::Vector2f(center.x, 0);
	}
	else if (m_isRightAligned)
	{
		newOrigin += sf::Vector2f(2 * center.x, 0);
	}
	if (m_centerVertical)
	{
		newOrigin += sf::Vector2f(0, center.y);
	}


	m_text.setOrigin(newOrigin);
}

sf::Color TextComponent::GetTextColor()
{
	return m_textColor;
}
