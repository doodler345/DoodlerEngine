#include "PlayScene.h"

#include <SFML/Network.hpp>

#include "../Player.h"

void PlayScene::SetWorld(std::string bmpFilePath)
{
	sf::UdpSocket socket;
	socket.bind(54000);
	socket.setBlocking(false);

	sf::Packet packet;
	char data[6] = "Hello";
	packet << data;
	socket.send(packet, sf::IpAddress::LocalHost, 54000);

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
