#pragma once
#include <iostream>
#include <string>
#include <map>
#include "SFML/Graphics.hpp"

struct Resource
{
	int m_id = -1;
	sf::Texture m_texture;
	sf::Font m_font;
};

class ResourceManager
{
public:
	void StartUp();
	void ShutDown();
	sf::Texture RegisterTexture(std::string path);
	sf::Texture UpdateTexture(std::string path);
	sf::Font RegisterFont(std::string path);
private:
	std::map<std::string, Resource> m_registeredTextures;
	std::map<std::string, Resource> m_registeredFonts;
	int m_idCounter = 0;
};
