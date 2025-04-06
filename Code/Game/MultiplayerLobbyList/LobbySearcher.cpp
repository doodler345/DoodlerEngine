#include "LobbySearcher.h"

void LobbySearcher::EntityInit()
{
	m_udpSocket.setBlocking(false);
	m_udpSocket.bind(m_PORT);
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
	sf::Packet lobbyPacket;
	sf::IpAddress sender;
	m_udpSocket.receive(lobbyPacket, sender, m_PORT);

	std::string lobbyName;
	if (lobbyPacket >> lobbyName)
	{
		std::cout << "Lobby found: " << lobbyName << std::endl;
	}
	else
	{
		//std::cout << "No lobby found." << std::endl;
	}
}
