#pragma once

#include "../Engine/Entity/Entity.h"

#include <SFML/Graphics.hpp>
#include <SFMLMath/SFMLMath.hpp>
#include <array>


class Player : public Entity
{
public:
	virtual void EntityInit() override;
	void SetMovementKeys(std::array<sf::Keyboard::Key, 4>&);


protected:
	virtual void DestroyDerived() override;
	virtual void Update(float deltaTime) override;
	void OnInputRecieved(sf::Keyboard::Key key, const bool keyDown);

private:
	float m_spriteSize = 0.3f;
	float m_moveSpeed = 200;
	const float m_rad45 = sf::degToRad(45);
	sf::Vector2f m_moveDirection = sf::Vector2f(0,0);
	std::array<sf::Keyboard::Key, 4> m_movementKeys;
};