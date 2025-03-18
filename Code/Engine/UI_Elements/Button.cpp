#include "Button.h"

#include "../Debug.h"
#include "../Engine.h"
#include "../InputManager.h"
#include "../UI_Elements/ButtonMenu.h"
#include "SFML/Graphics.hpp"


void Button::EntityInit()
{
	std::string path = "../Resources/Viga-Regular.ttf";
	m_buttonText = "default-button";

	std::shared_ptr<TextComponent> textComponent = std::make_shared<TextComponent>(path, m_buttonText, this);
	std::shared_ptr<SpriteComponent> spriteComponent = std::make_shared<SpriteComponent>(this);
	std::shared_ptr<RectangleComponent> rectangleComponent = std::make_shared<RectangleComponent>(this);

	AddComponent(textComponent);
	AddComponent(spriteComponent);
	AddComponent(rectangleComponent);
	
	m_textComponent = textComponent.get();
	m_spriteComponent = spriteComponent.get();
	m_rectangleComponent = rectangleComponent.get();

	m_rectangleComponent->GetRectangle()->setFillColor(sf::Color(220, 220, 220, 255));

	AdjustBackgroundToText();

	InputManager& inputManager = Engine::GetInstance()->GetInputManager();
	inputManager.RegisterRectangleEntry(this);
}

void Button::Update(float deltaTime)
{
	if (m_selected)
	{
		m_timer += deltaTime * m_selectedBlinkSpeed; 
		sf::Color textColor = m_textComponent->GetTextColor();
		textColor.a = 128 + cos(m_timer) * 128; 
		m_textComponent->SetTextColor(textColor);
	}
}

void Button::SetText(std::string text)
{
	m_textComponent->SetText(text);
	AdjustBackgroundToText();
}

void Button::SetFontSize(int fontSize)
{
	m_textComponent->SetFontSize(fontSize);
	AdjustBackgroundToText();
}

void Button::SetSprite(std::string texturePath)
{
	//cheap way to hide text & background
	SetText("");
	m_rectangleComponent->GetRectangle()->setFillColor(sf::Color(0, 0, 0, 0));
	m_rectangleComponent->GetRectangle()->setOutlineColor(sf::Color::Black);
	m_rectangleComponent->GetRectangle()->setOutlineThickness(2);

	m_spriteComponent->SetTexture(texturePath);

	AdjustBackgroundToSprite();

	m_hasSprite = true;
}

void Button::SetButtonCallback(ButtonCallback callback)
{
	m_buttonCallback = callback;
}

std::string Button::GetText()
{
	return m_textComponent->m_text.getString();
}

void Button::AdjustBackgroundToText()
{
	sf::Vector2f textSize = m_textComponent->m_text.getLocalBounds().getSize();
	float outerOffset = m_textComponent->GetFontSize() / 64.0f * 20; // TODO: this should be dynamic to text size
	m_rectangleComponent->GetRectangle()->setSize(sf::Vector2f(textSize.x + outerOffset, textSize.y + outerOffset));
	m_rectangleComponent->Center();
}

void Button::AdjustBackgroundToSprite()
{
	sf::Vector2f spriteSize = m_spriteComponent->m_drawable.getLocalBounds().getSize();
	m_rectangleComponent->GetRectangle()->setSize(sf::Vector2f(spriteSize.x, spriteSize.y));
	m_rectangleComponent->Center();
}

void Button::OnHover()
{
	if (!m_selected)
	{
		if (m_buttonMenu)
		{
			m_buttonMenu->SelectButton(this);
		}
		else
		{
			Select(true); //this is only working if just one button exists in a scene.
		}
	}
}

void Button::Select(bool isSelected)
{
	m_selected = isSelected;

	if (isSelected)
	{
		if (m_hasSprite)
		{
			m_rectangleComponent->GetRectangle()->setFillColor(sf::Color(0, 0, 0, 50));
		}
	}
	else
	{
		if (m_hasSprite)
		{
			m_rectangleComponent->GetRectangle()->setFillColor(sf::Color(0,0,0,0));
		}
		else
		{
			sf::Color textColor = m_textComponent->GetTextColor();
			textColor.a = 255;
			m_textComponent->SetTextColor(textColor);
			m_timer = 0;
		}
	}
}

bool Button::IsSelected()
{
	return m_selected;
}

void Button::Submit()
{
	if (!m_buttonCallback)
	{
		DebugPrint("No callbackFunction set", TextColor::Red, DebugChannel::UI, __FILE__, __LINE__, true);
	}
	else
	{
		m_buttonCallback();
	}
}

void Button::DestroyDerived()
{
	InputManager& inputManager = Engine::GetInstance()->GetInputManager();
	inputManager.UnregisterRectangleEntry(&GetTransformable());
}
