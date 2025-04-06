#pragma once

#include <string>
#include <SFML/Network.hpp>

//#include "../World.h"

struct Lobby
{
	int id;
	std::string name;
	std::string ipAddress;
	int port;
};

inline sf::Packet& operator<<(sf::Packet& packet, const Lobby& lobby)
{
	return packet << lobby.id << lobby.name << lobby.ipAddress << lobby.port;
}

inline sf::Packet& operator>>(sf::Packet& packet, Lobby& lobby)
{
	return packet >> lobby.id >> lobby.name >> lobby.ipAddress >> lobby.port;
}