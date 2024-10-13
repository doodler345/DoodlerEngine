#include "InputField.h"

#include "../Engine.h"
#include "../InputManager.h"

void InputField::EntityInit()
{
	std::string fontPath = "../Resources/Viga-Regular.ttf";
	m_inputText = "enter text...";
	m_fileEndingText = "";
	m_textOffset = sf::Vector2f(-m_inputFieldSize.x * 0.5f + 10, 0);

	std::shared_ptr<TextComponent> textComponent = std::make_shared<TextComponent>(fontPath, m_inputText, this, sf::Color::Black, false);
	std::shared_ptr<TextComponent> textComponentFileEnding = std::make_shared<TextComponent>(fontPath, m_fileEndingText, this, sf::Color::Black, false);
	std::shared_ptr<RectangleComponent> rectangleComponent = std::make_shared<RectangleComponent>(this);
	std::shared_ptr<RectangleComponent> rectangleMarker = std::make_shared<RectangleComponent>(this);

	textComponent->m_allowMultiple = true;
	textComponentFileEnding->m_allowMultiple = true;

	AddComponent(textComponent);
	AddComponent(textComponentFileEnding);
	AddComponent(rectangleComponent);
	AddComponent(rectangleMarker);

	m_textComponent = textComponent.get();
	m_textComponentFileEnding = textComponentFileEnding.get();
	m_rectangleComponent = rectangleComponent.get();
	m_rectangleMarker = rectangleMarker.get();

	m_textComponent->m_text.setPosition(m_textOffset);
	m_textComponentFileEnding->m_isRightAligned = true;
	m_textComponentFileEnding->m_text.setPosition(-m_textOffset);
	m_rectangleComponent->GetRectangle()->setFillColor(sf::Color(220, 220, 220, 255));
	m_rectangleComponent->GetRectangle()->setOutlineColor(sf::Color::Black);
	m_rectangleComponent->GetRectangle()->setOutlineThickness(2);
	m_rectangleComponent->GetRectangle()->setSize(m_inputFieldSize);
	m_rectangleComponent->Center();
	m_rectangleMarker->GetRectangle()->setPosition(sf::Vector2f(100, 0));
	m_rectangleMarker->GetRectangle()->setFillColor(sf::Color(0, 0, 0, 0));
	m_rectangleMarker->GetRectangle()->setSize(m_markerSize);
	m_rectangleMarker->Center(false);

	InputManager& inputManager = Engine::GetInstance()->GetInputManager();
	inputManager.RegisterRectangleEntry(this);
}


void InputField::Update(float deltaTime)
{
	if (m_isActive)
	{
		m_blinkTimer += deltaTime;
		if (m_blinkTimer > 0.70f)
		{
			m_blinkTimer = 0;
			sf::Color newMarkerColor = m_markerIsVisible ? sf::Color(0, 0, 0, 0) : m_markerColor;
			m_rectangleMarker->GetRectangle()->setFillColor(newMarkerColor);
			m_markerIsVisible = !m_markerIsVisible;
		}
	}

}

void InputField::Activate(bool setActive)
{
	if (setActive == false)
	{
		m_rectangleMarker->GetRectangle()->setFillColor(sf::Color(0, 0, 0, 0));
	}
	else
	{
		m_rectangleMarker->GetRectangle()->setFillColor(m_markerColor);
		m_blinkTimer = 0;
	}

	m_isActive = setActive;
}

void InputField::SetText(std::string text)
{
	m_inputText = text;
	UpdateText();
}

void InputField::SetFileEnding(std::string fileEnding)
{
	m_textComponentFileEnding->SetText(fileEnding);
}

void InputField::SetMaxChars(int maxChars)
{
	m_maxChars = maxChars;
}

void InputField::AppendCharacter(std::string newChar)
{
	if (m_inputText.size() < m_maxChars)
	{
		m_inputText += newChar;
		UpdateText();
	}
}

void InputField::RemoveCharacter()
{
	if (m_inputText.size() > 0)
	{
		m_inputText.pop_back();
		UpdateText();
	}
}

void InputField::UpdateText()
{
	m_textComponent->SetText(m_inputText);
	sf::Vector2f newMarkerPos = sf::Vector2f(m_textOffset.x + m_textComponent->m_text.getGlobalBounds().width, 0);
	m_rectangleMarker->GetRectangle()->setPosition(newMarkerPos);
}

void InputField::DestroyDerived()
{
	InputManager& inputManager = Engine::GetInstance()->GetInputManager();
	inputManager.UnregisterRectangleEntry(&GetTransform());
}