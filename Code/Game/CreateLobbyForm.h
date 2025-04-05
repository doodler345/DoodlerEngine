#pragma once

#include "../Engine/Entity/Entity.h"
#include "../Engine/EntityComponents/Primitives/RectangleComponent.h"

class CreateLobbyForm : public Entity
{
public:
	void EntityInit() override;
	void DestroyDerived() override;

	void SetActive(bool value);

private:
	std::shared_ptr <RectangleComponent> m_background = nullptr;

	sf::Vector2f m_RELATIVE_SIZE{ 0.7f, 0.7f }; // Relative size to the window size
};