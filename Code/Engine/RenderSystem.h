#pragma once

#include <iostream>
#include <memory>
#include <map>

#include "SFML/Graphics.hpp"
#include "Entity/Entity.h"

struct RenderEntry
{
	EntityComponent* m_owner = nullptr;
	sf::Drawable* m_drawable = nullptr;
	sf::Shader* m_shader = nullptr;
};

struct RenderLayer
{
	int layer;
	std::map<int, RenderEntry> m_spriteEntries;
	std::map<int, RenderEntry> m_textEntries;
	std::map<int, RenderEntry> m_shapeEntries;
	std::map<int, RenderEntry> m_shaderEntries;
};

enum EntryType
{
	SpriteEntry,
	TextEntry,
	ShapeEntry,
	ShaderEntry
};


class RenderSystem
{
public:
	void Init(sf::RenderWindow* renderWindow);
	void Update();
	
	void AddSprite(sf::Sprite* sprite, EntityComponent* owner, int layer = 0);
	void AddText(sf::Text* text, EntityComponent* owner, int layer = 0);
	void AddShape(sf::Shape* shape, EntityComponent* owner, int layer = 0);
	void AddShader(sf::Drawable* drawable, sf::Shader* shader, EntityComponent* owner, int layer = 0);
	
	//void RemoveEntry(EntityComponent* owner, EntryType type);
	void RemoveSprite(EntityComponent* owner);
	void RemoveText(EntityComponent* owner);
	void RemoveShape(EntityComponent* owner);
	void RemoveShader(EntityComponent* owner);

private:
	sf::RenderWindow* m_renderWindow = nullptr;
	std::map<EntityComponent*, int> m_ownerToLayer;
	std::map<int, RenderLayer*> m_renderLayers;
	std::vector<std::shared_ptr<RenderLayer>> m_renderLayersVector;
};