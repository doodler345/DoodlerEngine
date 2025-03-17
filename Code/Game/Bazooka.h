#include <iostream>

#include "SFML/Graphics.hpp"
#include "BazookaRocket.h"

class Bazooka
{
public:
	void Fire(sf::Vector2f direction, float strength);

private:
	BazookaRocket* m_rocket = nullptr;
};