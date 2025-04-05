#include "Player.h"

#include <../../Engine/Engine.h>
#include <../../Engine/InputManager.h>
#include <../../Engine/Debug.h>
#include <../../Engine/Scene.h>
#include <../../Engine/EntityComponents/Drawables/TextComponent.h>
#include "Scenes/PlayScene.h"
#include "GameManager.h"

#include <iostream>
#include <numbers>
#include <memory>
#include <functional>

void Player::EntityInit() 
{
	DebugPrint("Player " + std::to_string(m_id) + " spawned", TextColor::Green, DebugChannel::Entity, __FILE__, __LINE__);
	
	GameManager* gameManager = reinterpret_cast<GameManager*>(Engine::GetInstance()->GetGameManagerEntity());
	PlayScene* playScene = reinterpret_cast<PlayScene*>(gameManager->GetCurrentScene());
	m_world = playScene->GetWorld();
	assert(m_world);

	// Sprite  
	std::string pathTextureIdle = "../Resources/Sprites/Character_01.png";
	m_spriteComponent = std::make_shared<SpriteComponent>(pathTextureIdle, this);
	m_spriteComponent->m_drawable->setScale(m_SPRITE_SCALE, m_SPRITE_SCALE);
	m_spriteSize = m_spriteComponent->m_drawable->getGlobalBounds().getSize();
	AddComponent(m_spriteComponent);

	// Animator
	std::vector<std::string> pathTexturesWalk;
	pathTexturesWalk.push_back("../Resources/Sprites/Character_01.png");
	pathTexturesWalk.push_back("../Resources/Sprites/Character_02.png");
	pathTexturesWalk.push_back("../Resources/Sprites/Character_03.png");
	pathTexturesWalk.push_back("../Resources/Sprites/Character_04.png");
	pathTexturesWalk.push_back("../Resources/Sprites/Character_03.png");
	pathTexturesWalk.push_back("../Resources/Sprites/Character_02.png");
	m_animatorComponent = std::make_shared<AnimatorComponent>();
	m_animatorComponent->Setup(m_spriteComponent);
	m_animatorComponent->AddAnimation("Idle", { pathTextureIdle }, 0);
	m_animatorComponent->AddAnimation("Walk", pathTexturesWalk, 0.4f);
	m_animatorComponent->SetAnimation("Idle");
	AddComponent(m_animatorComponent);

	// Aim Direction
	pathTextureIdle = "../Resources/Sprites/AimDirection.png";
	m_aimDirectionHolder = playScene->Instantiate(Empty, AimDirectionHolder);
	m_aimDirectionHolder->SetParent(this);
	m_aimDirection = std::make_shared<SpriteComponent>(pathTextureIdle, m_aimDirectionHolder);
	m_aimDirection->m_drawable->setScale(m_SPRITE_SCALE + 0.5f, m_SPRITE_SCALE + 0.5f);
	m_aimDirectionHolder->AddComponent(m_aimDirection);

	// Bazooka
	m_bazooka = playScene->Instantiate(Bazooka, Bazooka);
	m_bazooka->SetOwner(this);
	m_bazooka->SetParent(m_aimDirectionHolder);

	// Calculating Player World Height
	m_worldPlayerSize = m_world->ScreenToWorldPosition(sf::Vector2u(m_spriteSize));
	m_worldVeritcalClimbingThreshold = m_worldPlayerSize.y * m_RELATIVE_WORLD_VERTICAL_CLIMBING_THRESHOLD;

	m_screenPlayerCollisionWidth = m_spriteSize.x * m_RELATIVE_COLLISION_WIDTH * 0.5f;
}

void Player::Setup(int playerNumber, std::array<sf::Keyboard::Key, 2>& movementKeys, std::array<sf::Keyboard::Key, 2>& aimKeys, sf::Keyboard::Key fireKey, bool lookToLeft)
{
	m_playerNumber = playerNumber;
	m_movementKeys = movementKeys;
	m_aimKeys = aimKeys;
	m_fireKey = fireKey;

	InputManager& inputManager = Engine::GetInstance()->GetInputManager();
	for (int i = 0; i < 2; i++)
	{
		inputManager.RegisterKeyboardEntry(movementKeys[i], std::bind(&Player::OnInputRecieved, this, std::placeholders::_1, std::placeholders::_2));
	}
	for (int i = 0; i < 2; i++)
	{
		inputManager.RegisterKeyboardEntry(aimKeys[i], std::bind(&Player::OnInputRecieved, this, std::placeholders::_1, std::placeholders::_2));
	}

	inputManager.RegisterKeyboardEntry(fireKey, std::bind(&Player::OnInputRecieved, this, std::placeholders::_1, std::placeholders::_2));

	if (lookToLeft)
	{
		m_aimDirectionHolder->GetTransformable().rotate(180);
	}
}

void Player::StopUpdate()
{
	m_isUpdateStopped = true;
}

void Player::DestroyDerived()
{
	InputManager& inputManager = Engine::GetInstance()->GetInputManager();
	for (int i = 0; i < 2; i++)
	{
		inputManager.UnregisterKeyboardEntry(m_movementKeys[i]);
	}
	for (int i = 0; i < 2; i++)
	{
		inputManager.UnregisterKeyboardEntry(m_aimKeys[i]);
	}

	inputManager.UnregisterKeyboardEntry(m_fireKey);
}

void Player::Update(float deltaTime)
{
	if (m_isUpdateStopped)
	{
		return;
	}

	if (!GroundedCheck())
	{
		m_velocity.x = 0;
		ApplyGravity(deltaTime);
		m_transformable.move(m_velocity);

		//m_aimDirection->SetVisibility(false);
	}
	else
	{
		//m_aimDirection->SetVisibility(true);
		m_velocity = sf::Vector2f(0,0);
		Move(deltaTime);

		RotateAimDirection(deltaTime);
	}

	m_transformable.move(m_velocity);
	//m_aimDirectionHolder->GetTransform().move(m_velocity);
	//std::cout << std::to_string(m_aimDirectionHolder->GetScreenPosition().x) + " " + std::to_string(m_aimDirectionHolder->GetScreenPosition().y) << std::endl;
}

void Player::OnInputRecieved(const sf::Keyboard::Key key, const bool keyDown)
{
	if (m_isUpdateStopped)
	{
		return;
	}

	int inverter = keyDown ? 1 : -1;

	//***** change to switch (caused an error, moveDirection should be "const", how to solve?)

	if (key == m_movementKeys[0])
	{
		m_inputMoveDirection.x += -1 * inverter;
	}
	else if (key == m_movementKeys[1])
	{
		m_inputMoveDirection.x += 1 * inverter;
	}

	if (key == m_aimKeys[0])
	{
		m_inputAimDirection.y += 1 * inverter;
	}
	else if (key == m_aimKeys[1])
	{
		m_inputAimDirection.y += -1 * inverter;
	}

	if (keyDown)
	{
		if (key == m_fireKey)
		{
			m_bazooka->PullTrigger();
		}
	}
	else
	{
		if (key == m_fireKey)
		{
			float aimDirectionRotation = m_aimDirectionHolder->GetTransformable().getRotation();
			sf::Vector2f shootDirection = sf::Vector2f(cosf(aimDirectionRotation / 360.f * 2 * std::numbers::pi), sinf(aimDirectionRotation / 360.f * 2 * std::numbers::pi));
			m_bazooka->Fire(shootDirection);
		}
	}
}

bool Player::GroundedCheck()
{
	sf::Vector2u worldPositionBeneath = m_world->ScreenToWorldPosition(GetScreenPosition()) + sf::Vector2u(0, m_worldPlayerSize.y * 0.5f);
	if (worldPositionBeneath.y >= m_world->m_worldHeight)
	{
		Scene* scene = reinterpret_cast<GameManager*>(Engine::GetInstance()->GetGameManagerEntity())->GetCurrentScene();
		reinterpret_cast<PlayScene*>(scene)->GameOver(m_playerNumber);
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
	m_velocity += sf::Vector2f(0, m_FALLSPEED) * deltaTime;
}

void Player::Move(float deltaTime)
{
	if (m_inputMoveDirection.x == 0)
	{
		if (m_walkType == WalkType::Walk)
		{
			m_walkType = WalkType::Idle;
			m_animatorComponent->SetAnimation("Idle");
		}
		return;
	}

	if (m_walkType == WalkType::Idle)
	{
		m_walkType = WalkType::Walk;
		m_animatorComponent->SetAnimation("Walk");
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

		if (worldPositionCheckHorizontalWall.y >= m_world->m_worldHeight || worldPositionCheckHorizontalWall.y < 0 || worldPositionCheckHorizontalWall.x >= m_world->m_worldWidth || worldPositionCheckHorizontalWall.x < 0)
		{
			continue;
		}

		if (*m_world->GetPixelValue(worldPositionCheckHorizontalWall) != 1) // If there is no wall
		{
			continue;
		}

		if (i >= m_worldVeritcalClimbingThreshold)
		{
			sf::Vector2i screenPositionCheckHorizontalWall = m_world->WorldToScreenPosition(sf::Vector2u(worldPositionCheckHorizontalWall + sf::Vector2u(0,1)));
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

	m_velocity = moveDirection * m_MOVESPEED * deltaTime;

	m_transformable.move(worldClimbValue);
}

void Player::RotateAimDirection(float deltaTime)
{
	if (m_inputAimDirection.y == 0)
	{
		return;
	}

	m_aimDirectionHolder->GetTransformable().rotate(-m_inputAimDirection.y * 100 * deltaTime);
}
