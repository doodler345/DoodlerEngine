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
	spriteComponent->m_drawable.setScale(m_SPRITE_SCALE, m_SPRITE_SCALE);
	m_spriteSize = spriteComponent->m_drawable.getGlobalBounds().getSize();
	AddComponent(spriteComponent);

	GameManager* gameManager = reinterpret_cast<GameManager*>(Engine::GetInstance()->GetGameManager());
	m_world = reinterpret_cast<PlayScene*>(gameManager->GetCurrentScene())->GetWorld();
	assert(m_world);

	m_bazooka = std::make_unique<Bazooka>();
	m_bazooka->SetOwner(this);

	// Calculating Player World Height
	m_worldPlayerSize = m_world->ScreenToWorldPosition(sf::Vector2u(m_spriteSize));
	m_worldVeritcalClimbingThreshold = m_worldPlayerSize.y * m_RELATIVE_WORLD_VERTICAL_CLIMBING_THRESHOLD;

	m_screenPlayerCollisionWidth = m_spriteSize.x * m_RELATIVE_COLLISION_WIDTH * 0.5f;
}

void Player::DestroyDerived()
{
	InputManager& inputManager = Engine::GetInstance()->GetInputManager();
	for (int i = 0; i < 2; i++)
	{
		inputManager.UnregisterKeyboardEntry(m_movementKeys[i]);
	}
}

void Player::SetInputKeys(std::array<sf::Keyboard::Key, 2>& movementKeys, sf::Keyboard::Key fireKey)
{
	m_movementKeys = movementKeys;
	m_fireKey = fireKey;

	InputManager& inputManager = Engine::GetInstance()->GetInputManager();
	for (int i = 0; i < 2; i++)
	{
		inputManager.RegisterKeyboardEntry(movementKeys[i], std::bind(&Player::OnInputRecieved, this, std::placeholders::_1, std::placeholders::_2));
	}

	inputManager.RegisterKeyboardEntry(fireKey, std::bind(&Player::OnInputRecieved, this, std::placeholders::_1, std::placeholders::_2));
}

void Player::OnInputRecieved(const sf::Keyboard::Key key, const bool keyDown)
{
	int inverter = keyDown ? 1 : -1;

	//***** change to switch (caused an error, moveDirection should be "const", how to solve?)

	if (key == m_movementKeys[0])
	{
		m_inputMoveDirection.x += -1 * inverter;
		return;
	}
	else if (key == m_movementKeys[1])
	{
		m_inputMoveDirection.x += 1 * inverter;
		return;
	}

	if (!keyDown)
	{
		return;
	}

	if (key == m_fireKey)
	{
		m_bazooka->Fire(sf::Vector2f(1, -1), 400);	
	}
}

void Player::Update(float deltaTime)
{
	if (!GroundedCheck())
	{
		ApplyGravity(deltaTime);
	}
	else
	{
		Move(deltaTime);
	}
}

bool Player::GroundedCheck()
{
	sf::Vector2u screenPositionBeneath = GetScreenPosition() + sf::Vector2u(0, m_spriteSize.y * 0.5f);

	sf::Vector2u worldPositionBeneath = m_world->ScreenToWorldPosition(screenPositionBeneath);
	if (m_world->ScreenToWorldPosition(screenPositionBeneath).y >= m_world->m_worldHeight)
	{
		return true;
	}

	float groundCheckCollisionWidth = m_worldPlayerSize.x * (m_RELATIVE_COLLISION_WIDTH + 0.1f); // NOTE: +0.1f to prevent be surely grounded after climbing (avoids insta-fall)
	for (int i = 0; i < groundCheckCollisionWidth; i++)
	{
		sf::Vector2u worldPositionBeneathWithXOffset = worldPositionBeneath + sf::Vector2u(groundCheckCollisionWidth * 0.5f - i, 0); // Checking from right to left

		if (*m_world->GetPixelValue(worldPositionBeneathWithXOffset) == 1)
		{
			return true;
		}
	}

	return false;
}

void Player::ApplyGravity(float deltaTime)
{
	//std::cout << "Falling";
	m_transform.translate(sf::Vector2f(0, 1) * m_FALLSPEED * deltaTime);
}

void Player::Move(float deltaTime)
{
	if (m_inputMoveDirection == sf::Vector2f(0, 0))
	{
		return;
	}

	sf::Vector2f moveDirection = m_inputMoveDirection;
	sf::Vector2u screenHorizontalDestination = GetScreenPosition() + sf::Vector2u(moveDirection.x * m_screenPlayerCollisionWidth,0);

	//Check if player is at the edge of the screen
	int windowWidth = Engine::GetInstance()->GetRenderWindow().getSize().x;
	if (moveDirection.x < 0 && (screenHorizontalDestination.x < 0 || screenHorizontalDestination.x > windowWidth * 2) || moveDirection.x > 0 && screenHorizontalDestination.x > windowWidth)
	{
		return;
	}

	// Check if player must / is able to climb
	sf::Vector2u worldHorizontalDestination = m_world->ScreenToWorldPosition(screenHorizontalDestination);
	sf::Vector2f worldClimbValue = sf::Vector2f(0,0);
	for (int i = 0; i < m_worldPlayerSize.y; i++)
	{
		int halfWorldPlayerSize = m_worldPlayerSize.y / 2.f;
		int verticalSampleHeight = -halfWorldPlayerSize + i; // Checking from top to bottom
		sf::Vector2u worldPositionCheckHorizontalWall = worldHorizontalDestination + sf::Vector2u(0, verticalSampleHeight);
		if (*m_world->GetPixelValue(worldPositionCheckHorizontalWall) != 1) // If there is no wall
		{
			continue;
		}

		if (i >= m_worldVeritcalClimbingThreshold)
		{
			sf::Vector2u screenPositionCheckHorizontalWall = m_world->WorldToScreenPosition(worldPositionCheckHorizontalWall + sf::Vector2u(0,1));
			float verticalClimbValue = (GetScreenPosition().y + 0.5*m_spriteSize.y) - screenPositionCheckHorizontalWall.y;
			worldClimbValue.y = -verticalClimbValue; 
			break;
		}
		else
		{
			moveDirection.x = 0;
			break;
		}
	}

	sf::Vector2f newScreenPosition = moveDirection * m_MOVESPEED * deltaTime + worldClimbValue;
	m_transform.translate(newScreenPosition);
}