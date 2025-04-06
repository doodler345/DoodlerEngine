#pragma once

#include <SFML/Network.hpp>

#include "../../Engine/Entity/Entity.h"
#include "../../Engine/UI_Elements/ButtonMenu.h"
#include "../../Engine/UI_Elements/Text.h"
#include "../../Engine/UI_Elements/InputField.h"
#include "../../Engine/EntityComponents/Drawables/RectangleComponent.h"
#include "../../Engine/Scene.h"
#include "../World.h"
#include "Lobby.h"

class CreateLobbyForm : public Entity
{
public:
	void EntityInit() override;
	void Update(float deltaTime) override;
	void DestroyDerived() override;

	void SetVisibility(bool value);

private:
	void CreateLobby();
	void BroadcastLobby(float deltaTime);

	sf::Vector2f m_RELATIVE_SIZE{ 0.7f, 0.7f }; // Relative size to the window size

	Scene* m_scene = nullptr;

	std::shared_ptr<RectangleComponent> m_background = nullptr;
	ButtonMenu* m_buttonMenu = nullptr;
	Text* m_waitForPlayerText = nullptr;
	InputField* m_lobbyNameInputField = nullptr;

	// Broadcast
	unsigned short m_port = 54000;
	unsigned short m_remotePort = 54001;
	World* m_world = nullptr;
	Lobby m_lobby;
	sf::IpAddress m_broadcastIp;
	sf::Packet m_lobbyPacket;
	sf::Packet m_worldPacket;
	sf::UdpSocket m_udpSocket;
	sf::TcpListener m_tcpListener;
	sf::TcpSocket m_tcpClientSocket;
	bool m_isBroadcastingLobby;

	float m_BROADCAST_INTERVAL_SECONDS = 1.0f;
	float m_timer = 0.0f;
};