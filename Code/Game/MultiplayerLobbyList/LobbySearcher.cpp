#include "LobbySearcher.h"

#include <../../Engine/Engine.h>
#include "../GameManager.h"
#include "../Scenes/MultiplayerLobbyListScene.h"
#include "Lobby.h"

void LobbySearcher::EntityInit()
{
	m_udpSocket.setBlocking(false);
	m_udpSocket.bind(m_port);

	m_multiplayerLobbyListScene = reinterpret_cast<MultiplayerLobbyListScene*>(reinterpret_cast<GameManager*>(Engine::GetInstance()->GetGameManagerEntity())->GetCurrentScene());
}

void LobbySearcher::Update(float deltaTime)
{
	m_timer += deltaTime;
	if (m_timer >= m_SEARCH_INTERVAL_SECONDS)
	{
		SearchForLobbies();
		m_timer = 0.0f;
	}
}

void LobbySearcher::DestroyDerived()
{
}

void LobbySearcher::SearchForLobbies()
{
	sf::Packet lobbyPacket;
	sf::IpAddress sender;

	// TODO: udpSocket manage multiple servers

	m_udpSocket.receive(lobbyPacket, sender, m_remotePort);

	Lobby lobby;
	if (lobbyPacket >> lobby)
	{
		if (m_activeLobbies.contains(lobby.id))
		{
			return;
		}

		m_activeLobbies[lobby.id] = lobby;

		m_multiplayerLobbyListScene->AddActiveLobby(&m_activeLobbies[lobby.id]);
		std::cout << "Lobby found: " << lobby.id << " " << lobby.ipAddress << std::endl;
	}
	else
	{
		if (!m_activeLobbies.empty())
		{
			m_multiplayerLobbyListScene->ClearActiveLobbies();
			m_activeLobbies.clear();
		}
	}
}
