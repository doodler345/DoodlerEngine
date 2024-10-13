#include "Player.h"

#include <../../Engine/Engine.h>
#include <../../Engine/InputManager.h>
#include <../../Engine/Debug.h>
#include "../../Engine/EntityComponents/SpriteComponent.h"
#include "../../Engine/EntityComponents/TextComponent.h"
#include <iostream>
#include <memory>
#include <functional>

void Player::EntityInit() 
{
	DebugPrint("Player " + std::to_string(m_id) + " spawned", TextColor::Green, DebugChannel::Entity, __FILE__, __LINE__);
	
	std::string path = "../Resources/Profilbild Sonic Infusion.png";
	std::shared_ptr<SpriteComponent> spriteComponent = std::make_shared<SpriteComponent>(path, this);
	spriteComponent->m_drawable.setScale(m_spriteSize, m_spriteSize);
	AddComponent(spriteComponent);	
}

void Player::DestroyDerived()
{
	InputManager& inputManager = Engine::GetInstance()->GetInputManager();
	for (int i = 0; i < 4; i++)
	{
		inputManager.UnregisterKeyboardEntry(m_movementKeys[i]);
	}
}

void Player::SetMovementKeys(std::array<sf::Keyboard::Key, 4>& keys)
{
	m_movementKeys = keys;
	InputManager& inputManager = Engine::GetInstance()->GetInputManager();
	for (int i = 0; i < 4; i++)
	{
		inputManager.RegisterKeyboardEntry(keys[i], std::bind(&Player::OnInputRecieved, this, std::placeholders::_1, std::placeholders::_2));
	}
}

void Player::OnInputRecieved(const sf::Keyboard::Key key, const bool keyDown)
{
	int inverter = keyDown ? 1 : -1;

	//***** change to switch (caused an error, moveDirection should be "const", how to solve?)
	if (key == m_movementKeys[0])
	{
		m_moveDirection.y += -1 * inverter;
	}
	else if (key == m_movementKeys[1])
	{
		m_moveDirection.x += -1 * inverter;
	}
	else if (key == m_movementKeys[2])
	{
		m_moveDirection.y += 1 * inverter;
	}
	else if (key == m_movementKeys[3])
	{
		m_moveDirection.x += 1 * inverter;
	}

}

void Player::Update(float deltaTime)
{
	if (m_moveDirection != sf::Vector2f(0, 0))
	{
		sf::Vector2f normalizedMoveDirection;
		if (m_moveDirection.x != 0 && m_moveDirection.y != 0)
		{
			//***** this is only valid for keyboard, not joystick!
			normalizedMoveDirection = sf::Vector2f(m_moveDirection.x * sin(m_rad45), m_moveDirection.y * cos(m_rad45));
		}
		else
		{
			normalizedMoveDirection = m_moveDirection;
		}

		m_transform.translate(normalizedMoveDirection * m_moveSpeed * deltaTime);
	}
}
