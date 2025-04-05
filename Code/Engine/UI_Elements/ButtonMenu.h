#pragma once

#include "SFML/Graphics.hpp"
#include <array>
#include <vector>
#include <memory>

#include "../Entity/Entity.h"
#include "Button.h"

class ButtonMenu : public Entity
{
public:
	virtual void EntityInit() override {};
	std::vector<Button*> InitMenu(int buttonCount, std::array<sf::Keyboard::Key, 3>& inputKeys);
	void SelectButton(Button* button);
	void Pause(bool isPaused);
	void SetVisibility(bool value);

private:
	void OnNextButton(sf::Keyboard::Key key, bool isPressed);
	void OnPrevButton(sf::Keyboard::Key key, bool isPressed);
	void OnSubmitSelected(sf::Keyboard::Key key, bool isPressed);
	virtual void DestroyDerived() override;

	std::array<sf::Keyboard::Key, 3> m_navigationKeys;
	std::vector<Button*> m_buttons;
	int m_buttonCount;
	int m_selectedButtonIndex;
};