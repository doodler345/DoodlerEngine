#pragma once

#include "../Engine/Entity/Entity.h"
#include "World.h"
#include "Bazooka.h"

#include <SFML/Graphics.hpp>
#include <SFMLMath/SFMLMath.hpp>
#include <array>


class Player : public Entity
{
public:
	virtual void EntityInit() override;
	void SetInputKeys(std::array<sf::Keyboard::Key, 2>& movementKeys, std::array<sf::Keyboard::Key, 2>& aimKeys, sf::Keyboard::Key fireKey);


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
	const float m_FALLSPEED = 300;
	const float m_MOVESPEED = 30;
	const float m_RELATIVE_WORLD_VERTICAL_CLIMBING_THRESHOLD = 0.6f;
	const float m_RELATIVE_COLLISION_WIDTH = 0.6f;
	const float m_SPRITE_SCALE = 1.5f;

	World* m_world = nullptr;
	Bazooka* m_bazooka = nullptr;
	std::shared_ptr<SpriteComponent> m_aimDirection = nullptr;
	sf::Vector2f m_spriteSize;
	sf::Vector2u m_worldPlayerSize;
	int m_screenPlayerCollisionWidth;
	int m_worldVeritcalClimbingThreshold;

	sf::Vector2f m_inputMoveDirection = sf::Vector2f(0,0);
	sf::Vector2f m_inputAimDirection = sf::Vector2f(0,0);
	bool m_isGrounded = false;

	
	std::array<sf::Keyboard::Key, 2> m_movementKeys;
	std::array<sf::Keyboard::Key, 2> m_aimKeys;
	sf::Keyboard::Key m_fireKey;
};
