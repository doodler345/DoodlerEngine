#pragma once

#include <iostream>
#include "../Entity/Entity.h"
#include "../ResourceManager.h"
#include "SFML/Graphics.hpp"

class SpriteComponent : public EntityComponent
{
public:
	SpriteComponent(Entity* owner);
	SpriteComponent(std::string& texturePath, Entity* owner);
	void SetTexture(std::string& texturePath);
	void UpdateTexture();
	void CenterTexture();
	void SetVisibility(bool isVisible);
	sf::Vector2u GetTextureSize();
	virtual void ShutDown() override;

	sf::Sprite m_drawable; 
private:
	bool m_isInitialized = false;
	bool m_isVisible = true;
	std::string m_texturePath = "";
	sf::Texture m_texture;
};