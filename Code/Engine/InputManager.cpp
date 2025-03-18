#include "InputManager.h"

#include "Debug.h"
#include "EntityComponents/TextComponent.h"
#include "EntityComponents/Primitives/RectangleComponent.h"
#include "UI_Elements/ButtonMenu.h"
#include "UI_Elements/InputFieldValidKeys.h"
#include <assert.h>


InputManager::InputManager()
{
	for (int i = 0; i < validInputFieldKeys.size(); i++)
	{
		ValidInputFieldKeyEntry newValidInputKey;
		newValidInputKey.m_key = validInputFieldKeys[i];
		m_validInputFieldKeys.push_back(newValidInputKey);
	}

	m_leftMouseButton.m_mouseButtonToListen = sf::Mouse::Left;
	m_middleMouseButton.m_mouseButtonToListen = sf::Mouse::Middle;
	m_rightMouseButton.m_mouseButtonToListen = sf::Mouse::Right;
	m_mouseButtonEntries[0] = &m_leftMouseButton;
	m_mouseButtonEntries[1] = &m_middleMouseButton;
	m_mouseButtonEntries[2] = &m_rightMouseButton;
}

void InputManager::Update(float mouseScrollDelta)
{
	sf::RenderWindow& window = Engine::GetInstance()->GetRenderWindow();
	if (!window.hasFocus()) return;

	// Check Mouse-Buttons 
	MouseButtonEntry* button;
	for (int i = 0; i < 3; i++)
	{
		button = m_mouseButtonEntries[i];

		if (button->m_released)
		{
			button->m_released = false;
		}

		if (IsMousePressed(button->m_mouseButtonToListen) && !button->m_pressed)
		{
			button->m_pressed = true;
			button->m_pressedDown = true;
		}
		else if (IsMousePressedDown(button->m_mouseButtonToListen) && IsMousePressed(button->m_mouseButtonToListen))
		{
			button->m_pressedDown = false;
		}
		else if (button->m_pressed && !IsMousePressed(button->m_mouseButtonToListen))
		{
			button->m_pressed = false;
			button->m_pressedDown = false;
			button->m_released = true;
		}
	}


	// Print current MousePos
	sf::Vector2u windowSize = window.getSize();
	sf::Vector2i mousePos = GetMousePos();
	int x = windowSize.x;
	int y = windowSize.y;
	if (IsMousePressedDown(sf::Mouse::Middle))
	{
		std::cout << mousePos.x << " " << mousePos.y << std::endl;
	}

	// Check if any Keys have been blocked previously through InputMode switch
	// e.g. Esc is used to get out of InputFieldMode and should not influence other Mode while still beeing pressed
	// so we wait until it is released again before it can be used in new mode
	for (int i = m_blockedKeysTillRelease.size() - 1; i >= 0; i--)
	{
		if (!IsKeyPressed(m_blockedKeysTillRelease[i]))
		{
			m_blockedKeysTillRelease.erase(m_blockedKeysTillRelease.begin() + i);
		}
	}


	// Rectangle Entries
	for (int i = 0; i < m_rectangleEntries.size(); i++)
	{
		RectangleEntry& entry = m_rectangleEntries[i];
		const sf::FloatRect floatRect = entry.m_rect->getGlobalBounds();
		sf::Vector2f entryPosition = entry.m_transformable->getPosition();

		// Hover-check (AABB)
		if (mousePos.x < (entryPosition.x - floatRect.left) &&	//right
			mousePos.x >(entryPosition.x + floatRect.left) &&	//left
			mousePos.y < (entryPosition.y - floatRect.top) &&	//down
			mousePos.y >(entryPosition.y + floatRect.top))		//up
		{
			if (!entry.m_hover)
			{
				entry.m_hover = true;

				if (entry.m_button)
				{
					entry.m_button->OnHover();
				}
			}


			if (IsMousePressedDown(entry.m_mouseButtonToListen) && !entry.m_pressed)
			{
				if (entry.m_inputField)
				{
					if (m_activeInputField)
					{
						m_activeInputField->Activate(false);
					}
					m_activeInputField = entry.m_inputField;
					m_activeInputField->Activate(true);
					SwitchInputMode(InputMode::InputFieldMode);
				}
				
				else // other rectangles
				{
					if (entry.m_button)
					{
						entry.m_button->Submit();
					}

					if (m_inputMode == InputMode::InputFieldMode)
					{
						SwitchInputMode(InputMode::DefaultMode);
					}
				}

				if (!entry.m_button)
				{
					entry.m_rectangleCallback(sf::Vector2f(mousePos), true);
				}
				entry.m_pressed = true;
				return;
			}
			else if (entry.m_pressed && IsMouseReleased(entry.m_mouseButtonToListen))
			{
				entry.m_rectangleCallback(sf::Vector2f(mousePos), false);
				entry.m_pressed = false;
			}
		}
		
		else // no hover
		{
			if (entry.m_hover)
			{
				entry.m_hover = false;
			}

			if (entry.m_pressed)
			{
				entry.m_pressed = false;
				entry.m_rectangleCallback(sf::Vector2f(mousePos), false);
			}
		}
	}





	switch (m_inputMode)
	{
	case InputMode::DefaultMode:

		// Key Inputs
		for (int i = 0; i < m_keyboardEntries.size(); i++)
		{
			KeyboardEntry& entry = m_keyboardEntries[i];
			bool keyIsBlocked = false;
			for (int k = 0; k < m_blockedKeysTillRelease.size(); k++)
			{
				if (m_blockedKeysTillRelease[k] == entry.m_keyToListen)
				{
					keyIsBlocked = true;
					break;
				}
			}
			if (keyIsBlocked)
				continue;

			if (IsKeyPressed(entry.m_keyToListen) && !entry.m_pressed)
			{
				entry.m_pressed = true;
				entry.m_keyboardCallback(entry.m_keyToListen, true);
			}
			else if (entry.m_pressed && !IsKeyPressed(entry.m_keyToListen))
			{
				entry.m_pressed = false;
				entry.m_keyboardCallback(entry.m_keyToListen, false);
			}
		}

		if (mouseScrollDelta != 0)
		{
			for (int i = 0; i < m_mouseScrollEntries.size(); i++)
			{
				m_mouseScrollEntries[i].m_mouseScrollCallback(mouseScrollDelta);
			}
		}

		break;

	case InputMode::InputFieldMode:

		// check if clicked out of InputField 
		sf::FloatRect rect = m_activeInputField->GetComponent<RectangleComponent>()->GetRectangle()->getGlobalBounds();
		sf::Vector2f entryPosition = m_activeInputField->GetTransformable().getPosition();


			// inverted Hover-check (AABB)
		if (mousePos.x >= (entryPosition.x - rect.left) ||	//right
			mousePos.x <= (entryPosition.x + rect.left) ||	//left
			mousePos.y >= (entryPosition.y - rect.top) ||	//down
			mousePos.y <  (entryPosition.y + rect.top))		//up
		{
			if (IsMousePressedDown(sf::Mouse::Left))
			{
				SwitchInputMode(InputMode::DefaultMode);
			}
		}


		for (int i = 0; i < m_validInputFieldKeys.size(); i++)
		{
			ValidInputFieldKeyEntry* inputFieldKeyEntry = &m_validInputFieldKeys[i];
			if (IsKeyPressed(inputFieldKeyEntry->m_key) && !inputFieldKeyEntry->m_pressed)
			{
				inputFieldKeyEntry->m_pressed = true;

				switch (inputFieldKeyEntry->m_key)
				{
				case KEY::Escape:
				case KEY::Enter:
					m_blockedKeysTillRelease.push_back(KEY::Escape);
					m_blockedKeysTillRelease.push_back(KEY::Enter);
					SwitchInputMode(InputMode::DefaultMode);
					break;
				case KEY::BackSpace:
					m_activeInputField->RemoveCharacter();
					break;
				default:
					bool shiftPressed = IsKeyPressed(KEY::LShift);
					m_activeInputField->AppendCharacter(KeyToString(inputFieldKeyEntry->m_key, shiftPressed));
					break;
				}
				break;
			}
			else if (!IsKeyPressed(inputFieldKeyEntry->m_key) && inputFieldKeyEntry->m_pressed)
			{
				inputFieldKeyEntry->m_pressed = false;
				break;
			}
		}
	}

}

void InputManager::RegisterKeyboardEntry(KEY keyOfInterest, KeyboardCallback callback)
{
	for (int i = 0; i < m_keyboardEntries.size(); i++)
	{
		if (m_keyboardEntries[i].m_keyToListen == keyOfInterest)
		{
			DebugPrint("Trying to register already existing KeyboardEntry " + m_keyboardEntries[i].m_id, TextColor::Red, DebugChannel::InputManager, __FILE__, __LINE__, 1); //***** get Key to string for Print
			return;
		}
	}
		
	KeyboardEntry entry;
	entry.m_name = KeyToString(keyOfInterest);
	entry.m_id = m_keyboardEntryIDCounter++;
	entry.m_keyToListen = keyOfInterest;
	entry.m_keyboardCallback = callback;
	m_keyboardEntries.push_back(entry);

	DebugPrint("Registered KeyboardEntry " + entry.m_name + " (id: " + std::to_string(entry.m_id) + ")", TextColor::Green, DebugChannel::InputManager, __FILE__, __LINE__); //***** get Key to string for Print
}

void InputManager::RegisterRectangleEntry(Button* button)
{
	RectangleEntry newEntry;
	newEntry.m_id = m_rectangleEntryIDCounter++;
	newEntry.m_mouseButtonToListen = sf::Mouse::Left;
	newEntry.m_button = button;
	newEntry.m_transformable = &button->GetTransformable();
	newEntry.m_rect = button->GetComponent<RectangleComponent>()->GetRectangle();
	newEntry.m_rectangleCallback = std::bind(&InputManager::OnEmptyRectangleCallback, this, std::placeholders::_1, std::placeholders::_2);
	m_rectangleEntries.push_back(newEntry);

	DebugPrint("Registered RectangleEntry (id: " + std::to_string(newEntry.m_id) + ") (Button)", TextColor::Green, DebugChannel::InputManager, __FILE__, __LINE__); //***** get Key to string for Print
}

void InputManager::RegisterRectangleEntry(InputField* inputField)
{
	RectangleEntry newEntry;
	newEntry.m_id = m_rectangleEntryIDCounter++;
	newEntry.m_mouseButtonToListen = sf::Mouse::Left;
	newEntry.m_inputField = inputField;
	newEntry.m_transformable = &inputField->GetTransformable();
	newEntry.m_rect = inputField->GetComponent<RectangleComponent>()->GetRectangle();
	newEntry.m_rectangleCallback = std::bind(&InputManager::OnEmptyRectangleCallback, this, std::placeholders::_1, std::placeholders::_2);
	m_rectangleEntries.push_back(newEntry);

	DebugPrint("Registered RectangleEntry (id: " + std::to_string(newEntry.m_id) + ") (InputField)", TextColor::Green, DebugChannel::InputManager, __FILE__, __LINE__); //***** get Key to string for Print
}

void InputManager::RegisterRectangleEntry(MOUSE::Button buttonOfInteresst, sf::Transformable* transformable, sf::RectangleShape* rect, RectangleCallback callback)
{
	RectangleEntry newEntry;
	newEntry.m_id = m_rectangleEntryIDCounter++;
	newEntry.m_mouseButtonToListen = buttonOfInteresst;
	newEntry.m_transformable = transformable;
	newEntry.m_rect = rect;
	newEntry.m_rectangleCallback = callback;
	m_rectangleEntries.push_back(newEntry);

	DebugPrint("Registered RectangleEntry (id: " + std::to_string(newEntry.m_id) + ") (Clickable)", TextColor::Green, DebugChannel::InputManager, __FILE__, __LINE__); //***** get Key to string for Print
}

void InputManager::RegisterMouseScrollEntry(sf::Transformable* transformable, MouseScrollCallback callback)
{
	MouseScrollListenerEntry newEntry;
	newEntry.m_id = m_mouseScrollEntryIDCounter++;
	newEntry.m_transformable = transformable;
	newEntry.m_mouseScrollCallback = callback;
	m_mouseScrollEntries.push_back(newEntry);

	DebugPrint("Registered MouseScrollEntry (id: " + std::to_string(newEntry.m_id) + ")", TextColor::Green, DebugChannel::InputManager, __FILE__, __LINE__); //***** get Key to string for Print
}

void InputManager::UnregisterKeyboardEntry(const KEY keyOfInterest)
{
	for (int i = 0; i < m_keyboardEntries.size(); i++)
	{
		if (m_keyboardEntries[i].m_keyToListen == keyOfInterest)
		{
			DebugPrint("Unregistered KeyboardEntry " + m_keyboardEntries[i].m_name + " (id: " + std::to_string(m_keyboardEntries[i].m_id) + ")", TextColor::Yellow, DebugChannel::InputManager, __FILE__, __LINE__); //***** get Key to string for Print
			m_keyboardEntries.erase(m_keyboardEntries.begin() + i);
			return;
		}
	}
	DebugPrint("No such Key to unregister!", TextColor::Red, DebugChannel::InputManager, __FILE__, __LINE__, 1); //***** get Key to string for Print
}

void InputManager::UnregisterRectangleEntry(sf::Transformable* transformable)
{
	for (int i = 0; i < m_rectangleEntries.size(); i++)
	{
		if (m_rectangleEntries[i].m_transformable == transformable)
		{
			DebugPrint("Unregistered RectangleEntry " + m_rectangleEntries[i].m_name + " (id: " + std::to_string(m_rectangleEntries[i].m_id) + ")", TextColor::Yellow, DebugChannel::InputManager, __FILE__, __LINE__);
			m_rectangleEntries.erase(m_rectangleEntries.begin() + i);
			return;
		}
	}
	DebugPrint("No such RectangleEntry to unregister!", TextColor::Red, DebugChannel::InputManager, __FILE__, __LINE__, 1); 
}

void InputManager::UnregisterMouseScrollEntry(sf::Transformable* transformable)
{
	for (int i = 0; i < m_mouseScrollEntries.size(); i++)
	{
		if (m_mouseScrollEntries[i].m_transformable == transformable)
		{
			DebugPrint("Unregistered MouseScrollEntry " + m_mouseScrollEntries[i].m_name + " (id: " + std::to_string(m_mouseScrollEntries[i].m_id) + ")", TextColor::Yellow, DebugChannel::InputManager, __FILE__, __LINE__);
			m_mouseScrollEntries.erase(m_mouseScrollEntries.begin() + i);
			return;
		}
	}
	DebugPrint("No such MouseScrollEntry to unregister!", TextColor::Red, DebugChannel::InputManager, __FILE__, __LINE__, 1); 
}

sf::Vector2i InputManager::GetMousePos()
{
	return sf::Mouse::getPosition(Engine::GetInstance()->GetRenderWindow());
}

bool InputManager::IsKeyPressed(KEY key)
{
	if (sf::Keyboard::isKeyPressed(key)) return true;
	else return false;
}

bool InputManager::IsMousePressed(sf::Mouse::Button button)
{
	if (sf::Mouse::isButtonPressed(button)) return true;
	else return false;
}

bool InputManager::IsMousePressedDown(sf::Mouse::Button button)
{
	switch (button)
	{
	case (sf::Mouse::Left):
		return m_leftMouseButton.m_pressedDown;
	case (sf::Mouse::Middle):
		return m_middleMouseButton.m_pressedDown;
	case (sf::Mouse::Right):
		return m_rightMouseButton.m_pressedDown;
	}
}

bool InputManager::IsMouseReleased(sf::Mouse::Button button)
{
	switch (button)
	{
	case (sf::Mouse::Left):
		return m_leftMouseButton.m_released;
	case (sf::Mouse::Middle):
		return m_middleMouseButton.m_released;
	case (sf::Mouse::Right):
		return m_rightMouseButton.m_released;
	}
}

void InputManager::SwitchInputMode(InputMode newInputMode)
{
	switch(newInputMode)
	{
	case InputMode::DefaultMode:

		m_activeInputField->Activate(false);
		m_activeInputField = nullptr;

		if (m_rectangleEntries.size() > 0)
		{
			for (int i = 0; i < m_rectangleEntries.size(); i++)
			{
				if (m_rectangleEntries[i].m_button)
				{
					ButtonMenu* buttonMenu = m_rectangleEntries[i].m_button->m_buttonMenu;
					if (buttonMenu)
					{
						buttonMenu->Pause(false);
					}
					else
					{
						m_rectangleEntries[i].m_button->Select(true);
					}
					break;
				}
			}
		}
		break;

	case InputMode::InputFieldMode:

		if (m_rectangleEntries.size() > 0)
		{
			for (int i = 0; i < m_rectangleEntries.size(); i++)
			{
				if (m_rectangleEntries[i].m_button)
				{
					ButtonMenu* buttonMenu = m_rectangleEntries[i].m_button->m_buttonMenu;
					if (buttonMenu)
					{
						buttonMenu->Pause(true);
					}
					else
					{
						m_rectangleEntries[i].m_button->Select(false);
					}
					break;
				}
			}
		}
		break;
	}

	m_inputMode = newInputMode;
}

std::string incomingString;
#define ITEM_LOWER(x) case KEY::##x : incomingString = #x; return std::string(1, std::tolower(*incomingString.data()));
#define ITEM_UPPER(x) case KEY::##x : return #x;
#define ITEM_NUMBER(x) case KEY::Num##x : return #x;
std::string InputManager::KeyToString(KEY key, bool upper)
{
	//general keys
	switch (key)
	{
		ITEM_NUMBER(0)
		ITEM_NUMBER(1)
		ITEM_NUMBER(2)
		ITEM_NUMBER(3)
		ITEM_NUMBER(4)
		ITEM_NUMBER(5)
		ITEM_NUMBER(6)
		ITEM_NUMBER(7)
		ITEM_NUMBER(8)
		ITEM_NUMBER(9)
		ITEM_UPPER(Enter)
		ITEM_UPPER(Backspace)
		ITEM_UPPER(Backslash)
		ITEM_UPPER(Escape)
		ITEM_UPPER(Left)
		ITEM_UPPER(Right)
		ITEM_UPPER(F1)
		ITEM_UPPER(F2)
		ITEM_UPPER(F3)
		ITEM_UPPER(F4)
		ITEM_UPPER(F5)
		ITEM_UPPER(F6)
		ITEM_UPPER(F7)
		ITEM_UPPER(F8)
		ITEM_UPPER(F9)
		ITEM_UPPER(F10)
		ITEM_UPPER(F11)
		ITEM_UPPER(F12)
		case KEY::Space: return " ";
	}

	if (upper)
	{
		switch (key)
		{
			ITEM_UPPER(A)
			ITEM_UPPER(B)
			ITEM_UPPER(C)
			ITEM_UPPER(D)
			ITEM_UPPER(E)
			ITEM_UPPER(F)
			ITEM_UPPER(G)
			ITEM_UPPER(H)
			ITEM_UPPER(I)
			ITEM_UPPER(J)
			ITEM_UPPER(K)
			ITEM_UPPER(L)
			ITEM_UPPER(M)
			ITEM_UPPER(N)
			ITEM_UPPER(O)
			ITEM_UPPER(P)
			ITEM_UPPER(Q)
			ITEM_UPPER(R)
			ITEM_UPPER(S)
			ITEM_UPPER(T)
			ITEM_UPPER(U)
			ITEM_UPPER(V)
			ITEM_UPPER(W)
			ITEM_UPPER(X)
			ITEM_UPPER(Y)
			ITEM_UPPER(Z)
			case KEY::Dash: return "_";
		}
	}
	else
	{
		switch (key)
		{
			ITEM_LOWER(A)
			ITEM_LOWER(B)
			ITEM_LOWER(C)
			ITEM_LOWER(D)
			ITEM_LOWER(E)
			ITEM_LOWER(F)
			ITEM_LOWER(G)
			ITEM_LOWER(H)
			ITEM_LOWER(I)
			ITEM_LOWER(J)
			ITEM_LOWER(K)
			ITEM_LOWER(L)
			ITEM_LOWER(M)
			ITEM_LOWER(N)
			ITEM_LOWER(O)
			ITEM_LOWER(P)
			ITEM_LOWER(Q)
			ITEM_LOWER(R)
			ITEM_LOWER(S)
			ITEM_LOWER(T)
			ITEM_LOWER(U)
			ITEM_LOWER(V)
			ITEM_LOWER(W)
			ITEM_LOWER(X)
			ITEM_LOWER(Y)
			ITEM_LOWER(Z)
			case KEY::Dash: return "-";
		}
	}
	
	
	
	DebugPrint("Couldn't find string to registered Key", TextColor::Red, DebugChannel::InputManager, __FILE__, __LINE__, true);
	return "unknown key";
}

void InputManager::OnEmptyRectangleCallback(sf::Vector2f v2, bool b)
{
}
