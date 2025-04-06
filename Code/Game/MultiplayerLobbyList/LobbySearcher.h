#pragma once

#include <string>
#include <SFML/Network.hpp>
#include <../../Engine/Entity/Entity.h>

class LobbySearcher : public Entity
{
public:
	void EntityInit() override;
	void Update(float deltaTime) override;
	void DestroyDerived() override;

private:
	void SearchForLobbies();

	unsigned short m_PORT = 54000;
	sf::UdpSocket m_udpSocket;
	std::string m_lobbyName;
};