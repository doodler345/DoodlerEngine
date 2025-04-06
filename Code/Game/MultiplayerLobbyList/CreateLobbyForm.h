#pragma once

#include <string>

#include <SFML/Network.hpp>

#include "../../Engine/Entity/Entity.h"
#include "../../Engine/UI_Elements/ButtonMenu.h"
#include "../../Engine/UI_Elements/Text.h"
#include "../../Engine/UI_Elements/InputField.h"
#include "../../Engine/EntityComponents/Drawables/RectangleComponent.h"
#include "../../Engine/Scene.h"
#include "../World.h"
#include "../LevelSelection.h"
#include "Lobby.h"

class CreateLobbyForm : public Entity
{
public:
	void EntityInit() override;
	void Update(float deltaTime) override;
	void DestroyDerived() override;
	void SetVisibility(bool value);

private:
	void ToggleSelectLevelPanel(bool value);
	void OnSelectLevel(std::string filePath, std::string fileName);
	void SetSelectedLevelPath(std::string filePath) { m_selectedWorldPath = filePath; }
	void CreateLobby();
	void BroadcastLobby(float deltaTime);

	const sf::Vector2f m_RELATIVE_SIZE{ 0.95f, 0.95f }; // Relative size to the window size

	Scene* m_scene = nullptr;
	std::shared_ptr<RectangleComponent> m_background = nullptr;
	ButtonMenu* m_buttonMenu = nullptr;
	Button* m_levelSelectionButton = nullptr;
	LevelSelection* m_levelSelection = nullptr;
	Text* m_waitForPlayerText = nullptr;
	InputField* m_lobbyNameInputField = nullptr;

	// Broadcast
	const float m_BROADCAST_INTERVAL_SECONDS = 1.0f;
	unsigned short m_port = 54000;
	unsigned short m_remotePort = 54001;
	std::string m_selectedWorldPath = "../Resources/bmp/DontDelete/DefaultMap.bmp";
	World* m_world = nullptr;
	Lobby m_lobby;
	sf::IpAddress m_broadcastIp;
	sf::Packet m_lobbyPacket;
	sf::Packet m_worldPacket;
	sf::UdpSocket m_udpSocket;
	sf::TcpListener m_tcpListener;
	sf::TcpSocket m_tcpClientSocket;
	bool m_isBroadcastingLobby;

	float m_timer = 0.0f;
};