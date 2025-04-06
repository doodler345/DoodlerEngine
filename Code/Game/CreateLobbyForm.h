#pragma once

#include <SFML/Network.hpp>

#include "../Engine/Entity/Entity.h"
#include "../Engine/UI_Elements/ButtonMenu.h"
#include "../Engine/UI_Elements/Text.h"
#include "../Engine/EntityComponents/Drawables/RectangleComponent.h"
#include "../Engine/Scene.h"
#include "World.h"

class CreateLobbyForm : public Entity
{
public:
	void EntityInit() override;
	void Update(float deltaTime) override;
	void DestroyDerived() override;

	void SetActive(bool value);

private:
	void CreateLobby();
	void BroadcastLobby();

	sf::Vector2f m_RELATIVE_SIZE{ 0.7f, 0.7f }; // Relative size to the window size

	Scene* m_scene = nullptr;

	std::shared_ptr<RectangleComponent> m_background = nullptr;
	ButtonMenu* m_buttonMenu = nullptr;
	Text* m_waitForPlayerText = nullptr;

	// Broadcast
	World* m_broadcastWorld = nullptr;
	sf::Packet m_packet;
	sf::TcpListener m_tcpListener;
	sf::TcpSocket m_tcpClientSocket;
	bool m_isBroadcastingLobby;
};