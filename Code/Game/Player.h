#pragma once

#include "../Engine/Entity/Entity.h"
#include "World.h"

#include <SFML/Graphics.hpp>
#include <SFMLMath/SFMLMath.hpp>
#include <array>


class Player : public Entity
{
public:
	virtual void EntityInit() override;
	void SetMovementKeys(std::array<sf::Keyboard::Key, 2>&);


protected:
	virtual void DestroyDerived() override;
	virtual void Update(float deltaTime) override;

private:
	void OnInputRecieved(sf::Keyboard::Key key, const bool keyDown);
	bool GroundedCheck();
	void ApplyGravity(float deltaTime);
	void Move(float deltaTime);

	const float m_RAD45 = sf::degToRad(45);
	const float m_FALLSPEED = 100;
	const float m_MOVESPEED = 30;
	const float m_RELATIVE_WORLD_VERTICAL_CLIMBING_THRESHOLD = 0.6f;
	const float m_RELATIVE_COLLISION_WIDTH = 0.6f;

	World* world = nullptr;
	sf::Vector2f m_spriteSize;
	sf::Vector2u m_worldPlayerSize;
	float m_spriteScale = 1.5f;
	int m_screenPlayerCollisionWidth;
	int m_worldVeritcalClimbingThreshold;
	bool m_isGrounded = false;
	sf::Vector2f m_inputMoveDirection = sf::Vector2f(0,0);
	std::array<sf::Keyboard::Key, 2> m_movementKeys;
};
