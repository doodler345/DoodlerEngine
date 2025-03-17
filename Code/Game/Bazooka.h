#include <iostream>

#include "SFML/Graphics.hpp"
#include "BazookaRocket.h"

class Bazooka
{
public:
	void Fire(sf::Vector2f direction, float strength);
	void SetOwner(Entity* owner) { m_owner = owner; }

private:
	Entity* m_owner = nullptr;
	BazookaRocket* m_rocket = nullptr;
};