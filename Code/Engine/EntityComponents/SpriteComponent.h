#pragma once

#include <iostream>
#include "../Entity/Entity.h"
#include "../Entity/EntityComponent.h"
#include "../ResourceManager.h"
#include "SFML/Graphics.hpp"

class SpriteComponent : public DrawableEntityComponent
{
public:
	SpriteComponent(Entity* owner, int renderLayer = 0);
	SpriteComponent(std::string& texturePath, Entity* owner, int renderLayer = 0);
	void SetTexture(std::string& texturePath);
	void UpdateTexture();
	void CenterTexture();
	void SetVisibility(bool value);
	sf::Vector2u GetTextureSize();
	virtual void ShutDown() override;

	sf::Sprite m_drawable; 
private:
	bool m_isInitialized = false;
	std::string m_texturePath = "";
	sf::Texture m_texture;
};