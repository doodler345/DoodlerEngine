#pragma once

#include <iostream>
#include <string>
#include <functional>
#include "SFML/Graphics.hpp"
#include "UI_Elements/Button.h"
#include "UI_Elements/InputField.h"

using KEY = sf::Keyboard::Key;
using MOUSE = sf::Mouse;
using KeyboardCallback = std::function<void(KEY, bool)>;
using RectangleCallback = std::function<void(sf::Vector2f, bool)>;
using MouseScrollCallback = std::function<void(float)>;

////////////////////////////////////////////
struct Entry
{
	std::string m_name;
	int m_id = -1;
	bool m_pressed = false;
};

struct KeyboardEntry : Entry
{
	KEY m_keyToListen;
	KeyboardCallback m_keyboardCallback;
};

struct ValidInputFieldKeyEntry : Entry
{
	KEY m_key;
};

struct MouseButtonEntry : Entry
{
	MOUSE::Button m_mouseButtonToListen;
	bool m_pressedDown = false;
	bool m_released = false;
};

struct MouseScrollListenerEntry : Entry
{
	sf::Transform* m_transform;
	MouseScrollCallback m_mouseScrollCallback;
};

////////////////////////////////////////////

struct RectangleEntry : Entry
{
	MOUSE::Button m_mouseButtonToListen;
	sf::Transform* m_transform;
	sf::RectangleShape* m_rect;
	RectangleCallback m_rectangleCallback;
	Button* m_button = nullptr;
	InputField* m_inputField = nullptr;
	bool m_hover = false;
};


enum InputMode
{
	DefaultMode,
	InputFieldMode
};


class InputManager
{
public:
	InputManager();

	void Update(float mouseScrollDelta);

	void RegisterKeyboardEntry(KEY keyOfInterest, KeyboardCallback callback);
	void RegisterRectangleEntry(Button* button);
	void RegisterRectangleEntry(InputField* inputField);
	void RegisterRectangleEntry(MOUSE::Button buttonOfInteresst, sf::Transform* transform, sf::RectangleShape* rect, RectangleCallback callback);
	void RegisterMouseScrollEntry(sf::Transform* transform, MouseScrollCallback callback);
	
	void UnregisterKeyboardEntry(KEY keyOfInterest);
	void UnregisterRectangleEntry(sf::Transform* transform); 
	void UnregisterMouseScrollEntry(sf::Transform* transform); 

	sf::Vector2i GetMousePos();

private:
	bool IsKeyPressed(KEY key);
	bool IsMousePressed(sf::Mouse::Button button);
	bool IsMousePressedDown(sf::Mouse::Button button);
	bool IsMouseReleased(sf::Mouse::Button button);

	void SwitchInputMode(InputMode newInputMode);

	std::string KeyToString(KEY key, bool upper = true);
	void OnEmptyRectangleCallback(sf::Vector2f v2, bool b);
	
	InputMode m_inputMode = InputMode::DefaultMode;

	int m_keyboardEntryIDCounter = 0;
	int m_rectangleEntryIDCounter = 0;
	int m_mouseScrollEntryIDCounter = 0;
	
	std::vector<KeyboardEntry> m_keyboardEntries;
	std::vector<RectangleEntry> m_rectangleEntries;
	std::vector<MouseScrollListenerEntry> m_mouseScrollEntries;

	MouseButtonEntry* m_mouseButtonEntries[3];
	MouseButtonEntry m_leftMouseButton;
	MouseButtonEntry m_middleMouseButton;
	MouseButtonEntry m_rightMouseButton;

	InputField* m_activeInputField = nullptr;

	std::vector<ValidInputFieldKeyEntry> m_validInputFieldKeys;

	std::vector<KEY> m_blockedKeysTillRelease;
};
