#pragma once

#include <iostream>
#include <functional>
#include "SFML/Graphics.hpp"

#include "../Entity/InteractableUIEntity.h"
#include "../EntityComponents/TextComponent.h"
#include "../EntityComponents/SpriteComponent.h"
#include "../EntityComponents/Primitives/RectangleComponent.h"

class ButtonMenu;

using ButtonCallback = std::function<void()>;

class Button : public InteractableUIEntity
{
public:
	virtual void EntityInit() override;
	virtual void Update(float deltaTime) override;
	void SetText(std::string text);
	void SetFontSize(int fontSize);
	void SetSprite(std::string texturePath);
	void SetButtonCallback(ButtonCallback callback);

	std::string GetText();

	void OnHover();
	void Select(bool isSelected);
	bool IsSelected();
	void Submit();

	ButtonMenu* m_buttonMenu = nullptr;

private:
	void AdjustBackgroundToText();
	void AdjustBackgroundToSprite();
	virtual void DestroyDerived() override;
	ButtonCallback m_buttonCallback;
	TextComponent* m_textComponent = nullptr;
	RectangleComponent* m_rectangleComponent = nullptr;
	SpriteComponent* m_spriteComponent = nullptr;
	std::string m_buttonText;
	bool m_hasSprite = false;
	bool m_selected = true;
	float m_timer = 0;

	//customizations
	float m_selectedBlinkSpeed = 5.f;

	// Inherited via InteractableUIEntity
	void SetInteractable(bool isInteractable) override;
};