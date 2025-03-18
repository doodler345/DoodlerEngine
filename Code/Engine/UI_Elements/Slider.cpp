#include "Slider.h"

#include <memory>
#include <functional>
#include <SFML/Graphics.hpp>
#include "../InputManager.h"

void Slider::EntityInit()
{
	std::shared_ptr<RectangleComponent> rectangleComponentBackground = std::make_shared<RectangleComponent>(this);

	AddComponent(rectangleComponentBackground);

	m_rectangleBackground = rectangleComponentBackground.get();

	m_rectangleBackground->GetRectangle()->setFillColor(sf::Color(150, 150, 150, 255));
	m_rectangleBackground->Center();
}

void Slider::Setup(Empty* handlerOwner, sf::Vector2f size, ValueChangedCallback callback)
{
	InputManager& inputManager = Engine::GetInstance()->GetInputManager();

	m_rectangleHandlerOwner = handlerOwner;
	valueChangedCallback = callback;

	std::shared_ptr<RectangleComponent> rectangleComponetHandler = std::make_shared<RectangleComponent>(m_rectangleHandlerOwner);
	m_rectangleHandler = rectangleComponetHandler.get();
	m_rectangleHandlerOwner->AddComponent(rectangleComponetHandler);

	m_rectangleHandler->GetRectangle()->setFillColor(sf::Color(200, 200, 200, 255));

	m_rectangleBackground->GetRectangle()->setSize(size);
	m_rectangleHandler->GetRectangle()->setSize(sf::Vector2f(size.y, size.y));

	m_rectangleBackground->Center();
	m_rectangleHandler->Center();

	inputManager.RegisterRectangleEntry(sf::Mouse::Button::Left, &m_rectangleHandlerOwner->GetTransformable(), m_rectangleHandler->GetRectangle(), std::bind(&Slider::OnClick, this, std::placeholders::_1, std::placeholders::_2));
}

void Slider::Translate(float x, float y)
{
	GetTransformable().move(x, y);
	m_rectangleHandlerOwner->GetTransformable().move(x, y);
}

void Slider::SetValue(float relValue)
{
	if (relValue < 0 || relValue > 1) return;

	m_currentValue = relValue;

	sf::Vector2u handlerPos = m_rectangleHandlerOwner->GetScreenPosition();
	const sf::Vector2u backgroundPos = GetScreenPosition();
	const sf::Vector2f backgroundSize = m_rectangleBackground->GetRectangle()->getSize();
	const sf::Vector2f handlerSize = m_rectangleHandler->GetRectangle()->getSize();
	int possibleTravelAreaX = backgroundSize.x - handlerSize.x;

	float handlerTargetLocationX = backgroundPos.x - 0.5f * backgroundSize.x + 0.5f * handlerSize.x + possibleTravelAreaX * relValue;
	m_rectangleHandlerOwner->GetTransformable().move(sf::Vector2f(handlerTargetLocationX - handlerPos.x, 0));

	valueChangedCallback(m_currentValue);
}


void Slider::Update(float deltaTime)
{
	if (m_getsDragged)
	{
		m_mousePos = Engine::GetInstance()->GetInputManager().GetMousePos();
		m_deltaMousePos = m_mousePos - m_prevMousePos;
		m_prevMousePos = m_mousePos;

		if (m_deltaMousePos.x == 0.0f && m_deltaMousePos.y == 0.0f) return;

		sf::Vector2u handlerPos = m_rectangleHandlerOwner->GetScreenPosition();
		const sf::Vector2u backgroundPos = GetScreenPosition();
		const sf::Vector2f handlerSize = m_rectangleHandler->GetRectangle()->getSize();
		const sf::Vector2f backgroundSize = m_rectangleBackground->GetRectangle()->getSize();

		sf::Vector2f sliderNextPos = (sf::Vector2f)m_deltaMousePos;

		// TODO: Check if handler at bounds
		if (handlerPos.x - 0.5f * handlerSize.x + m_deltaMousePos.x < backgroundPos.x - 0.5f * backgroundSize.x)
		{
			sliderNextPos.x = (handlerPos.x - 0.5f * handlerSize.x) - (backgroundPos.x - 0.5f * backgroundSize.x);
		}
		else if	(handlerPos.x + 0.5f * handlerSize.x + m_deltaMousePos.x > backgroundPos.x + 0.5f * backgroundSize.x)
		{
			sliderNextPos.x = (backgroundPos.x + 0.5f * backgroundSize.x) - (handlerPos.x + 0.5f * handlerSize.x);
		}

		m_rectangleHandlerOwner->GetTransformable().move(sf::Vector2f(sliderNextPos.x, 0));

		handlerPos = m_rectangleHandlerOwner->GetScreenPosition();
		int possibleTravelAreaX = backgroundSize.x - handlerSize.x; 
		float currentZeroedHandlerPosX = handlerPos.x - (backgroundPos.x - 0.5f * backgroundSize.x + 0.5f * handlerSize.x);
		m_currentValue = currentZeroedHandlerPosX / possibleTravelAreaX;
		
		valueChangedCallback(m_currentValue);
	}
}

void Slider::OnClick(sf::Vector2f mousePos, bool isPressedDown)
{
	m_getsDragged = isPressedDown;

	if (isPressedDown)
	{
		m_prevMousePos = Engine::GetInstance()->GetInputManager().GetMousePos();
	}
}

void Slider::DestroyDerived()
{
	InputManager& inputManager = Engine::GetInstance()->GetInputManager();
	inputManager.UnregisterRectangleEntry(&m_rectangleHandlerOwner->GetTransformable());
	//inputManager.UnregisterRectangleEntry(&m_rectangleBackground->GetTransform());
}
