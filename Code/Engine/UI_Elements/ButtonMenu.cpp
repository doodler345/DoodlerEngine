#include "ButtonMenu.h"

#include "../Engine.h"
#include "../InputManager.h"
#include "../Entity/EntitySystem.h"
#include "../Entity/EntityFactoryManager.h"


std::vector<Button*> ButtonMenu::InitMenu(int buttonCount, std::array<sf::Keyboard::Key, 3>& inputKeys)
{
	this->m_buttonCount = buttonCount;
	m_navigationKeys = inputKeys;

	InputManager& inputManager = Engine::GetInstance()->GetInputManager();
	inputManager.RegisterKeyboardEntry
		(m_navigationKeys[0], std::bind(&ButtonMenu::OnPrevButton, this, std::placeholders::_1, std::placeholders::_2));
	inputManager.RegisterKeyboardEntry
		(m_navigationKeys[1], std::bind(&ButtonMenu::OnNextButton, this, std::placeholders::_1, std::placeholders::_2));
	inputManager.RegisterKeyboardEntry
		(m_navigationKeys[2], std::bind(&ButtonMenu::OnSubmitSelected, this, std::placeholders::_1, std::placeholders::_2));

	EntityFactoryManager* entityFactoryManager = EntityFactoryManager::GetInstance();
	for (int i = 0; i < buttonCount; i++)
	{
		Button* newButton = (Button*)entityFactoryManager->CreateEntity("Button", "ButtonMenu-Button");
		if (i == 0)
		{
			newButton->Select(true);
			m_selectedButtonIndex = 0;
		}
		else
		{
			newButton->Select(false);
		}
		newButton->m_buttonMenu = this;
		m_buttons.push_back(newButton);
	}
	return m_buttons;
}

void ButtonMenu::SelectButton(Button* button)
{
	for (int i = 0; i < m_buttons.size(); i++)
	{
		if (m_buttons[i] == button)
		{
			m_buttons[m_selectedButtonIndex]->Select(false);
			m_selectedButtonIndex = i;
			m_buttons[m_selectedButtonIndex]->Select(true);
			return;
		}
	}

	DebugPrint("Couldn't find Button " + std::to_string(button->GetID()), TextColor::Red, DebugChannel::UI, __FILE__, __LINE__, true);
}

void ButtonMenu::Pause(bool isPaused)
{
	m_buttons[m_selectedButtonIndex]->Select(!isPaused);
}

void ButtonMenu::OnNextButton(sf::Keyboard::Key key, bool isPressed)
{
	if (!isPressed) return;

	m_buttons[m_selectedButtonIndex]->Select(false);

	m_selectedButtonIndex++;
	m_selectedButtonIndex %= m_buttonCount;

	m_buttons[m_selectedButtonIndex]->Select(true);
}

void ButtonMenu::OnPrevButton(sf::Keyboard::Key key, bool isPressed)
{
	if (!isPressed) return;

	m_buttons[m_selectedButtonIndex]->Select(false);

	m_selectedButtonIndex--;
	if (m_selectedButtonIndex < 0)
	{
		m_selectedButtonIndex = m_buttonCount - 1;
	}

	m_buttons[m_selectedButtonIndex]->Select(true);
}

void ButtonMenu::OnSubmitSelected(sf::Keyboard::Key key, bool isPressed)
{
	if (!isPressed) return;

	m_buttons[m_selectedButtonIndex]->Submit();
}

void ButtonMenu::DestroyDerived()
{
	EntitySystem& entitySystem = Engine::GetInstance()->GetEntitySystem();
	for (int i = 0; i < m_buttons.size(); i++)
	{
		entitySystem.DestroyEntity(m_buttons[i]);
	}
	m_buttons.clear();

	InputManager& inputManager = Engine::GetInstance()->GetInputManager();
	for (int i = 0; i < 3; i++)
	{
		inputManager.UnregisterKeyboardEntry(m_navigationKeys[i]);
	}
}