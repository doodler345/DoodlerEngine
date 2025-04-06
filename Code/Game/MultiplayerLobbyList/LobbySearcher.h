#pragma once

#include <string>
#include <map>
#include <SFML/Network.hpp>
#include <../../Engine/Entity/Entity.h>
#include "../MultiplayerLobbyList/Lobby.h"

class MultiplayerLobbyListScene;

class LobbySearcher : public Entity
{
public:
	void EntityInit() override;
	void Update(float deltaTime) override;

	void DestroyDerived() override;

private:
	void SearchForLobbies();

	MultiplayerLobbyListScene* m_multiplayerLobbyListScene = nullptr;

	const float m_SEARCH_INTERVAL_SECONDS = 1.0f;
	float m_timer = 0.0f;

	unsigned short m_port = 54001;
	unsigned short m_remotePort = 54000;
	sf::UdpSocket m_udpSocket;

	std::map<int, Lobby> m_activeLobbies;
};