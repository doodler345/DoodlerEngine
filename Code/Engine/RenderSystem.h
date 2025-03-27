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
	
	void AddEntry(sf::Drawable* drawable, EntityComponent* owner, EntryType type, int layer = 0, sf::Shader* shader = nullptr);	
	void RemoveEntry(EntityComponent* owner, EntryType type);

private:
	sf::RenderWindow* m_renderWindow = nullptr;
	std::map<EntityComponent*, int> m_ownerToLayer;
	std::map<int, RenderLayer*> m_renderLayers;
	std::vector<std::shared_ptr<RenderLayer>> m_renderLayersVector;
};