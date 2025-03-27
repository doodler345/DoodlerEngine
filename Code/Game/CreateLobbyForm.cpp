#include "CreateLobbyForm.h"

#include <SFML/Graphics.hpp>
#include "../Engine/Engine.h"

void CreateLobbyForm::EntityInit()
{
	sf::Vector2u windowSize = Engine::GetInstance()->GetRenderWindow().getSize();
	m_background = std::make_shared<RectangleComponent>(this);
	m_background->GetRectangle()->setSize(sf::Vector2f(windowSize.x * m_RELATIVE_SIZE.x, windowSize.y * m_RELATIVE_SIZE.y));
	m_background->Center();
	m_background->GetOwner()->GetTransformable().move(windowSize.x / 2, windowSize.y / 2);
}

void CreateLobbyForm::DestroyDerived()
{
}
