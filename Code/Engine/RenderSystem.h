#pragma once

#include <iostream>
#include <vector>
#include "SFML/Graphics.hpp"
#include "Entity/Entity.h"

struct RenderEntry
{
	Entity* m_owner = nullptr;
	sf::Drawable* m_drawable = nullptr;
	sf::Shader* m_shader = nullptr;
};


class RenderSystem
{
public:
	void Update();
	void AddSprite(sf::Sprite* sprite, Entity* owner);
	void RemoveSprite(Entity* owner);
	
	void AddText(sf::Text* text, Entity* owner);
	void RemoveText(Entity* owner);

	void AddShape(sf::Shape* shape, Entity* owner);
	void RemoveShape(Entity* owner);

	void AddShader(sf::Drawable* drawable, sf::Shader* shader, Entity* owner);
	void RemoveShader(Entity* owner);

private:
	
	std::vector<RenderEntry> m_spriteEntries;
	std::vector<RenderEntry> m_textEntries;
	std::vector<RenderEntry> m_shapeEntries;
	std::vector<RenderEntry> m_shaderEntries;
};