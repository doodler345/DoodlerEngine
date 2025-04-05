#pragma once

#include <memory>

#include "SFML/Graphics.hpp"
#include "../../Entity/Entity.h"

class RectangleComponent : public DrawableEntityComponent
{
public:
	RectangleComponent(Entity* owner, int renderLayer = 0);

	// TODO: Rectangles Position not really moved yet! It just gets rendered at owners position
	sf::RectangleShape* GetRectangle() { return m_drawable.get(); }
	void Center(bool horizontal = true, bool vertical = true);
	void SetVisibility(bool value) override;
	void SetRenderLayer(int layer) override;
	bool m_isRightAligned = false;

private:
	std::unique_ptr<sf::RectangleShape> m_drawable;
	void ShutDown() override;
};
