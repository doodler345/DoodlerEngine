#pragma once

#include "../Engine/Entity/Entity.h"
#include "../Engine/Entity/Empty.h"
#include "../Engine/EntityComponents/SpriteComponent.h"
#include "../Engine/EntityComponents/AnimatorComponent.h"

#include "World.h"
#include "Bazooka.h"

#include <SFML/Graphics.hpp>
#include <SFMLMath/SFMLMath.hpp>
#include <array>

enum WalkType
{
	Idle,
	Walk
};

class Player : public Entity
{
public:
	virtual void EntityInit() override;
	void Setup(int playerNumber, std::array<sf::Keyboard::Key, 2>& movementKeys, std::array<sf::Keyboard::Key, 2>& aimKeys, sf::Keyboard::Key fireKey, bool lookToLeft = false);
	void StopUpdate();

protected:
	virtual void DestroyDerived() override;
	virtual void Update(float deltaTime) override;

private:
	void OnInputRecieved(sf::Keyboard::Key key, const bool keyDown);
	bool GroundedCheck();
	void ApplyGravity(float deltaTime);
	void Move(float deltaTime);
	void RotateAimDirection(float deltaTime);

	const float m_RAD45 = sf::degToRad(45);
	const float m_FALLSPEED = 0.2f;
	const float m_MOVESPEED = 30;
	const float m_RELATIVE_WORLD_VERTICAL_CLIMBING_THRESHOLD = 0.6f;
	const float m_RELATIVE_COLLISION_WIDTH = 0.6f;
	const float m_SPRITE_SCALE = 1.5f;

	bool m_isUpdateStopped = false;

	World* m_world = nullptr;
	Bazooka* m_bazooka = nullptr;
	Empty* m_aimDirectionHolder = nullptr;

	std::shared_ptr<SpriteComponent> m_spriteComponent = nullptr;
	std::shared_ptr<AnimatorComponent> m_animatorComponent = nullptr;
	std::shared_ptr<SpriteComponent> m_aimDirection = nullptr;

	WalkType m_walkType = WalkType::Idle;

	sf::Vector2f m_spriteSize;
	sf::Vector2u m_worldPlayerSize;
	int m_playerNumber;
	int m_screenPlayerCollisionWidth;
	int m_worldVeritcalClimbingThreshold;

	sf::Vector2f m_inputMoveDirection = sf::Vector2f(0,0);
	sf::Vector2f m_inputAimDirection = sf::Vector2f(0,0);
	sf::Vector2f m_velocity;
	bool m_isGrounded = false;
	float m_dragTimer = 0;

	std::array<sf::Keyboard::Key, 2> m_movementKeys;
	std::array<sf::Keyboard::Key, 2> m_aimKeys;
	sf::Keyboard::Key m_fireKey;
};
