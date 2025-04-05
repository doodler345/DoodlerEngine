#pragma once

#include "../Engine/Entity/Entity.h"
#include "../Engine/UI_Elements/ButtonMenu.h"
#include "../Engine/UI_Elements/Text.h"
#include "../Engine/EntityComponents/Drawables/RectangleComponent.h"
#include "../Engine/Scene.h"

class CreateLobbyForm : public Entity
{
public:
	void EntityInit() override;
	void DestroyDerived() override;

	void SetActive(bool value);

private:
	void CreateLobby();

	Scene* m_scene = nullptr;

	std::shared_ptr<RectangleComponent> m_background = nullptr;
	ButtonMenu* m_buttonMenu = nullptr;
	Text* m_waitForPlayerText = nullptr;

	sf::Vector2f m_RELATIVE_SIZE{ 0.7f, 0.7f }; // Relative size to the window size
};