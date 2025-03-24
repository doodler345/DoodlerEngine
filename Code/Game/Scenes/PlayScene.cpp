#include "PlayScene.h"

#include <SFML/Network.hpp>

#include "../Player.h"

void PlayScene::SetWorld(std::string bmpFilePath)
{

	//Create World Entity
	sf::RenderWindow& window = Engine::GetInstance()->GetRenderWindow();

	World* world = Instantiate(World, GameWorld);
	assert(world);

	std::string strPath = bmpFilePath;
	const char* path = strPath.c_str();
	world->Setup(path);

	world->GetTransformable().move(window.getSize().x / 2, window.getSize().y / 2);
	world->UpdateTexture();
	world->Colorize(true);
	m_world = world;

	//sf::UdpSocket socket;
	unsigned short port = 54000;
	//socket.bind(port);
	//socket.setBlocking(false);

	sf::Packet packet;
	//std::string localAddress = sf::IpAddress::getLocalAddress().toString();
	//packet << localAddress;
	packet << "Hello";
	
	//int i = localAddress.length() - 1;
	//while (localAddress[i] != '.')
	//{
	//	i--;
	//}
	//std::string broadcastAddress = localAddress;
	//broadcastAddress.erase(i + 1, localAddress.length() - 1);
	//broadcastAddress.append("255");

	//sf::IpAddress recipientIP = sf::IpAddress(broadcastAddress);

	//socket.send(packet, recipientIP, port);

	sf::TcpListener listener;
	listener.listen(port);

	sf::TcpSocket clientSocket;
	listener.accept(clientSocket);

	clientSocket.send(packet);
}

World* PlayScene::GetWorld()
{
	return m_world;
}

void PlayScene::Init()
{
	m_name = "PlayScene";

	//Create Game-Entities
	sf::RenderWindow& window = Engine::GetInstance()->GetRenderWindow();

	//Player 1
	Player* player = Instantiate(Player, PlayerOne);
	assert(player);

	std::array<KEY, 2> movementKeysPlayer1 =
	{
		KEY::A,
		KEY::D,
	};
	std::array<KEY, 2> aimKeysPlayer1 =
	{
		KEY::W,
		KEY::S,
	};

	player->SetInputKeys(movementKeysPlayer1, aimKeysPlayer1, KEY::Space);
	player->GetTransformable().move(sf::Vector2f(window.getSize().x / 5, window.getSize().y / 5));


	//Player 2
	//Player* player2 = Instantiate(Player, PlayerTwo);
	//assert(player2);

	//std::array<KEY, 2> inputKeysPlayer2 =
	//{
	//	KEY::Left,
	//	KEY::Right,
	//};
	//player2->SetMovementKeys(inputKeysPlayer2);
	//player2->GetTransform().translate(sf::Vector2f(window.getSize().x - window.getSize().x / 5, window.getSize().y / 5));
}
