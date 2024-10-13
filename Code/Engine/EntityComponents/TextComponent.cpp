#include "TextComponent.h"
#include "../RenderSystem.h"
#include "../Engine.h"


TextComponent::TextComponent(std::string& path, std::string& text, Entity* owner, sf::Color color, bool centerHorizontal, bool centerVertical) :
	m_fontPath(path), m_textColor(color), m_centerHorizontal(centerHorizontal), m_centerVertical(centerVertical)
{
	m_ownerEntity = owner;

	ResourceManager& resourceManager = Engine::GetInstance()->GetResourceManager();
	RenderSystem& renderSystem = Engine::GetInstance()->GetRenderSystem();
		
	m_font = resourceManager.RegisterFont(m_fontPath);
	m_text.setFont(m_font);
	m_text.setFillColor(m_textColor);
	m_text.setString(text);
	SetFontSize(20);
	UpdateText();

	renderSystem.AddText(&m_text, owner);
}

void TextComponent::ShutDown()
{
	Engine::GetInstance()->GetRenderSystem().RemoveText(m_ownerEntity);
}

void TextComponent::SetText(std::string text)
{
	m_text.setString(text);
	UpdateText();
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

	UpdateText();
}

void TextComponent::SetFontSize(int fontSize)
{
	m_text.setCharacterSize(fontSize);
	this->m_fontSize = fontSize;
	UpdateText();
}

void TextComponent::UpdateText()
{
	if (!m_centerHorizontal && !m_centerVertical)
	{
		DebugPrint("Text cannot be centered, because bool is set to false!", TextColor::Red, DebugChannel::EntityComponent, __FILE__, __LINE__, true);
		return;
	}

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
