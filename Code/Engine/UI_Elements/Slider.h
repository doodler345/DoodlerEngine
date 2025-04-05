#pragma once

#include <functional>

#include "../Entity/InteractableUIEntity.h"
#include "../EntityComponents/Primitives/RectangleComponent.h"
#include "../Entity/Empty.h"

using ValueChangedCallback = std::function<void(float)>;

class Slider : public InteractableUIEntity
{
public:
	void EntityInit() override;
	void Setup(Empty* handlerOwner, sf::Vector2f size, ValueChangedCallback callback);
	void Translate(float x, float y);
	void SetValue(float relValue);
private:
	void Update(float deltaTime) override;
	void OnClick(sf::Vector2f mousePos, bool isPressedDown);
	void DestroyDerived() override;
	RectangleComponent* m_rectangleBackground = nullptr;
	RectangleComponent* m_rectangleHandler = nullptr;
	Empty* m_rectangleHandlerOwner = nullptr;

	bool m_getsDragged = false;

	ValueChangedCallback valueChangedCallback;
	sf::Vector2i m_prevMousePos;
	sf::Vector2i m_mousePos;
	sf::Vector2i m_deltaMousePos;

	float m_currentValue;

	// Inherited via InteractableUIEntity
	void SetInteractable(bool isInteractable) override;
};

