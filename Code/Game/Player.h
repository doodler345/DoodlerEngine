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
	sf::Vector2u ScreenToWorldPosition(sf::Vector2u screenPosition);
	bool GroundedCheck();
	void ApplyGravity(float deltaTime);
	void Move(float deltaTime);

	const float m_rad45 = sf::degToRad(45);
	const float m_fallSpeed = 100;
	const float m_moveSpeed = 100;

	World* world = nullptr;
	sf::Vector2f m_spriteSize;
	float m_spriteScale = 0.05f;
	bool m_isGrounded = false;
	sf::Vector2f m_moveDirection = sf::Vector2f(0,0);
	std::array<sf::Keyboard::Key, 2> m_movementKeys;
};
