#pragma once

#include <memory>

#include "SFML/Graphics.hpp"
#include "../../Entity/Entity.h"

class RectangleComponent : public EntityComponent
{
public:
	RectangleComponent(Entity* owner);

	// TODO: Rectangles Position not really moved yet! It just gets rendered at owners position
	sf::RectangleShape* GetRectangle() { return m_drawable.get(); }
	void Center(bool horizontal = true, bool vertical = true);
	bool m_isRightAligned = false;

private:
	std::unique_ptr<sf::RectangleShape> m_drawable;
	void ShutDown() override;
};
