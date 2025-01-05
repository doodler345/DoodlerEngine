#include "Player.h"

#include <../../Engine/Engine.h>
#include <../../Engine/InputManager.h>
#include <../../Engine/Debug.h>
#include <../../Engine/Scene.h>
#include "../../Engine/EntityComponents/SpriteComponent.h"
#include "../../Engine/EntityComponents/TextComponent.h"
#include "Scenes/PlayScene.h"
#include "GameManager.h"
#include "World.h"

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
	if (!GroundedCheck())
	{
		ApplyGravity(deltaTime);
	}
	Move(deltaTime);
}

bool Player::GroundedCheck()
{
	GameManager* gameManager = reinterpret_cast<GameManager*>(Engine::GetInstance()->GetGameManager());
	World* world = reinterpret_cast<PlayScene*>(gameManager->GetCurrentScene())->GetWorld();
	assert(world);

	sf::Vector2u windowSize = Engine::GetInstance()->GetRenderWindow().getSize();
	
	sf::Vector2f windowPositionRelative;
	windowPositionRelative.x = GetScreenPosition().x / (float)windowSize.x;
	sf::Vector2f textureSize = GetComponent<SpriteComponent>()->m_drawable.getGlobalBounds().getSize();
	windowPositionRelative.y = (GetScreenPosition().y + textureSize.y * 0.5f) / (float)windowSize.y;

	sf::Vector2u worldPosition;
	worldPosition.x = windowSize.x * windowPositionRelative.x;
	worldPosition.y = windowSize.y * windowPositionRelative.y;

	if (world->m_pixelValues[world->m_worldWidth * worldPosition.y + worldPosition.x] == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Player::ApplyGravity(float deltaTime)
{
	// if not grounded
	float fallSpeed = 40;
	m_transform.translate(sf::Vector2f(0, 1) * fallSpeed * deltaTime);
}

void Player::Move(float deltaTime)
{
	if (m_moveDirection == sf::Vector2f(0, 0))
	{
		return;
	}

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