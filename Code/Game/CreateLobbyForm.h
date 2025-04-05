#pragma once

#include "../Engine/Entity/Entity.h"
#include "../Engine/UI_Elements/ButtonMenu.h"
#include "../Engine/EntityComponents/Drawables/RectangleComponent.h"
#include "../Engine/Scene.h"

class CreateLobbyForm : public Entity
{
public:
	void EntityInit() override;
	void DestroyDerived() override;

	void SetActive(bool value);

private:
	Scene* m_scene = nullptr;

	std::shared_ptr<RectangleComponent> m_background = nullptr;
	ButtonMenu* m_buttonMenu = nullptr;

	sf::Vector2f m_RELATIVE_SIZE{ 0.7f, 0.7f }; // Relative size to the window size
};