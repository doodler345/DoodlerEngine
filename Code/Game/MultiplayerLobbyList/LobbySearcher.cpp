#include "LobbySearcher.h"

void LobbySearcher::EntityInit()
{
	m_udpSocket.setBlocking(false);
}

void LobbySearcher::Update(float deltaTime)
{
	SearchForLobbies();
}

void LobbySearcher::DestroyDerived()
{
}

void LobbySearcher::SearchForLobbies()
{
	sf::Packet m_lobbyPacket;
	sf::IpAddress sender;
	m_udpSocket.receive(m_lobbyPacket, sender, m_PORT);

	if (m_lobbyPacket >> m_lobbyName)
	{
		std::cout << "Lobby found: " << m_lobbyName << std::endl;
	}
	else
	{
		//std::cout << "No lobby found." << std::endl;
	}
}
