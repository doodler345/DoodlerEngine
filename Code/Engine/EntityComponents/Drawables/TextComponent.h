#pragma once

#include <iostream>
#include "../../Entity/Entity.h"
#include "../../Entity/EntityComponent.h"
#include "../../ResourceManager.h"
#include "SFML/Graphics.hpp"

class TextComponent : public DrawableEntityComponent
{
public:
	TextComponent(std::string& path, std::string& text, Entity* owner, sf::Color color = sf::Color::Black, bool centerHorizontal = true, bool centerVertical = true, int renderLayer = 0);

	void SetVisibility(bool value) override;
	void SetRenderLayer(int layer) override;
	virtual void ShutDown() override;

	void SetText(std::string newText);
	void SetTextColor(sf::Color color);
	void SetFont(std::string& path);
	void SetFontSize(int factor);
	int GetFontSize() { return m_fontSize; }

	sf::Color GetTextColor();

	sf::Text m_text; 
	bool m_isRightAligned = false;
	bool m_centerHorizontal = true;
	bool m_centerVertical = true;

private:
	void CenterText();

	std::string m_fontPath;
	sf::Font m_font;
	sf::Color m_textColor;
	int m_fontSize = 0;
};