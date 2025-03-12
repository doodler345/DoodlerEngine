#include "Player.h"

#include <../../Engine/Engine.h>
#include <../../Engine/InputManager.h>
#include <../../Engine/Debug.h>
#include <../../Engine/Scene.h>
#include "../../Engine/EntityComponents/SpriteComponent.h"
#include "../../Engine/EntityComponents/TextComponent.h"
#include "Scenes/PlayScene.h"
#include "GameManager.h"

#include <iostream>
#include <memory>
#include <functional>

void Player::EntityInit() 
{
	DebugPrint("Player " + std::to_string(m_id) + " spawned", TextColor::Green, DebugChannel::Entity, __FILE__, __LINE__);
	
	std::string path = "../Resources/Profilbild Sonic Infusion.png";
	std::shared_ptr<SpriteComponent> spriteComponent = std::make_shared<SpriteComponent>(path, this);
	spriteComponent->m_drawable.setScale(m_spriteScale, m_spriteScale);
	m_spriteSize = spriteComponent->m_drawable.getGlobalBounds().getSize();
	AddComponent(spriteComponent);

	GameManager* gameManager = reinterpret_cast<GameManager*>(Engine::GetInstance()->GetGameManager());
	world = reinterpret_cast<PlayScene*>(gameManager->GetCurrentScene())->GetWorld();
	assert(world);
}

void Player::DestroyDerived()
{
	InputManager& inputManager = Engine::GetInstance()->GetInputManager();
	for (int i = 0; i < 2; i++)
	{
		inputManager.UnregisterKeyboardEntry(m_movementKeys[i]);
	}
}

void Player::SetMovementKeys(std::array<sf::Keyboard::Key, 2>& keys)
{
	m_movementKeys = keys;
	InputManager& inputManager = Engine::GetInstance()->GetInputManager();
	for (int i = 0; i < 2; i++)
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
		m_moveDirection.x += -1 * inverter;
	}
	else if (key == m_movementKeys[1])
	{
		m_moveDirection.x += 1 * inverter;
	}
}

sf::Vector2u Player::ScreenToWorldPosition(sf::Vector2u screenPosition)
{
	sf::Vector2u windowSize = Engine::GetInstance()->GetRenderWindow().getSize();

	sf::Vector2f windowPositionRelative;
	windowPositionRelative.x = screenPosition.x / (float)windowSize.x;
	windowPositionRelative.y = screenPosition.y / (float)windowSize.y;

	sf::Vector2u worldPosition;
	worldPosition.x = windowSize.x * windowPositionRelative.x;
	worldPosition.y = windowSize.y * windowPositionRelative.y;

	return worldPosition;
}

void Player::Update(float deltaTime)
{
	if (!GroundedCheck())
	{
		ApplyGravity(deltaTime);
	}
	else
	{
	}
		Move(deltaTime);
}

bool Player::GroundedCheck()
{
	//TODO: Check for more beneath-pixels

	sf::Vector2u screenPositionBeneath = GetScreenPosition();
	screenPositionBeneath.y += m_spriteSize.y * 0.5f;

	sf::Vector2u worldPosition = ScreenToWorldPosition(screenPositionBeneath);

	int beneathPixelValue = *world->GetPixelValue(worldPosition);
	if (beneathPixelValue == 1)
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
	m_transform.translate(sf::Vector2f(0, 1) * m_fallSpeed * deltaTime);
}

void Player::Move(float deltaTime)
{
	if (m_moveDirection == sf::Vector2f(0, 0))
	{
		return;
	}

	sf::Vector2f normalizedMoveDirection;

	normalizedMoveDirection = m_moveDirection;

	// TODO: CLEAN UP

	sf::Vector2u screenPositionHorizontalMoveDirection = GetScreenPosition();
	screenPositionHorizontalMoveDirection += sf::Vector2u(normalizedMoveDirection.x * m_spriteSize.x * 0.5f,0);
	sf::Vector2u worldPositionHorizontalMoveDirection = ScreenToWorldPosition(screenPositionHorizontalMoveDirection);

	sf::Vector2u screenPositionSpriteHeightOffset = GetScreenPosition();
	screenPositionSpriteHeightOffset.y -= m_spriteSize.y;
	sf::Vector2u worldPositionHeightOffset = ScreenToWorldPosition(screenPositionSpriteHeightOffset);
	sf::Vector2u worldPosition = ScreenToWorldPosition(GetScreenPosition());

	int playerWorldHeight = worldPosition.y - worldPositionHeightOffset.y;
	int worldVericalClimbingThreshold = 0.6f * playerWorldHeight;
	float worldClimbValue = 0;
	for (int i = 0; i < playerWorldHeight; i++)
	{
		sf::Vector2u worldPositionVerticalOffset = sf::Vector2u(0, -playerWorldHeight / 2) + sf::Vector2u(0, i);
		sf::Vector2u worldPositionCheckHorizontalWall = worldPositionHorizontalMoveDirection + worldPositionVerticalOffset;
		if (*world->GetPixelValue(worldPositionCheckHorizontalWall) != 1)
		{
			continue;
		}

		if (i >= worldVericalClimbingThreshold)
		{
			// Todo: WorldToScreenPosition for climb value
			worldClimbValue = 1;
			break;
		}
		else
		{
			normalizedMoveDirection.x = 0;
			break;
		}
	}

	sf::Vector2f newScreenPosition = normalizedMoveDirection * m_moveSpeed * deltaTime - sf::Vector2f(0, worldClimbValue);
	m_transform.translate(newScreenPosition);
}