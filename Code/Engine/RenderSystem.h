#pragma once

#include <iostream>
#include <map>
#include "SFML/Graphics.hpp"
#include "Entity/Entity.h"

struct RenderEntry
{
	EntityComponent* m_owner = nullptr;
	sf::Drawable* m_drawable = nullptr;
	sf::Shader* m_shader = nullptr;
};


class RenderSystem
{
public:
	void Update();
	
	void AddSprite(sf::Sprite* sprite, EntityComponent* owner);
	void AddText(sf::Text* text, EntityComponent* owner);
	void AddShape(sf::Shape* shape, EntityComponent* owner);
	void AddShader(sf::Drawable* drawable, sf::Shader* shader, EntityComponent* owner);
	
	void RemoveSprite(EntityComponent* owner);
	void RemoveText(EntityComponent* owner);
	void RemoveShape(EntityComponent* owner);
	void RemoveShader(EntityComponent* owner);

private:
	
	std::map<int, RenderEntry> m_spriteEntries;
	std::map<int, RenderEntry> m_textEntries;
	std::map<int, RenderEntry> m_shapeEntries;
	std::map<int, RenderEntry> m_shaderEntries;
};