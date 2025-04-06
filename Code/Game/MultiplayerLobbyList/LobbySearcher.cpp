#include "LobbySearcher.h"

#include "Lobby.h"

void LobbySearcher::EntityInit()
{
	m_udpSocket.setBlocking(false);
	m_udpSocket.bind(m_port);
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
	m_udpSocket.receive(lobbyPacket, sender, m_remotePort);

	Lobby lobby;
	if (lobbyPacket >> lobby)
	{
		std::cout << "Lobby found: " << lobby.id << " " << lobby.ipAddress << std::endl;
	}
	else
	{
		//std::cout << "No lobby found." << std::endl;
	}
}
