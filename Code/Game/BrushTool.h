#pragma once

#include "memory"
#include "SFML/Graphics.hpp"


#include "../Engine/Engine.h"
#include "../Engine/EntityComponents/Primitives/CircleComponent.h"
#include "../Engine/Entity/Entity.h"
#include "../Engine/InputManager.h"
#include "../Engine/UI_Elements/Text.h"
#include "World.h"

class BrushTool : public Entity
{
public:
	void EntityInit() override;
	void Setup(World* worldGenerationImage, sf::Uint8* pixelColors, int* pixelEmpty, sf::Texture* texture, int windowHeight, int windowWidth, Text* radiusInfo);

	void Activate(bool isActive);
	void Draw(bool isDrawing);
	void Erase(bool eraseMode);
	
	void ChangeRadius(float add);
	int GetRadius() { return m_radius; }

	void DestroyDerived() override;

protected:
	void Update(float deltaTime) override;

private:
	bool m_isActive		= false;
	bool m_isDrawing	= false;
	bool m_eraseMode	= false;

	World* m_worldGenerationImage		= nullptr;
	sf::Uint8* m_pixelColors			= nullptr;
	int* m_pixelEmpty					= nullptr; // can cause issues when reallocating vector it points to!
	sf::Texture* m_texture				= nullptr;
	Text* m_radiusInfo					= nullptr;

	CircleComponent* m_circleComponent;
	sf::Color m_circleColor = sf::Color::Blue;

	sf::Vector2i m_prevMousePos;
	sf::Vector2i m_mousePos;
	sf::Vector2i m_deltaMousePos;

	const int m_outlineThickness = 4;
	const int m_scrollBoost = 5000;
	const int m_minBrushRadius = 4;
	const int m_maxBrushRadius = 400;
	float m_radius = 50;
	int m_windowWidth;
	int m_windowHeight;
};

