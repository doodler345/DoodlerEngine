#include "PlayScene.h"

#include <string>

#include "../Player.h"

World* PlayScene::GetWorld()
{
	return m_world;
}

void PlayScene::Init()
{
	m_name = "PlayScene";

	sf::RenderWindow& window = Engine::GetInstance()->GetRenderWindow();

	//Create Game-Entities
	//World
	World* world = Instantiate(World, GameWorld);
	assert(world);

	world->Setup("../Resources/bmp/DontDelete/GeneratedMap.bmp");
	world->GetTransform().translate(window.getSize().x / 2, window.getSize().y / 2);
	world->UpdateTexture();
	m_world = world;

	//Player 1
	Player* player = Instantiate(Player, PlayerOne);
	assert(player);

	std::array<KEY, 4> inputKeysPlayer1 =
	{
		KEY::W,
		KEY::A,
		KEY::S,
		KEY::D,
	};
	player->SetMovementKeys(inputKeysPlayer1);
	player->GetTransform().translate(sf::Vector2f(window.getSize().x / 5, window.getSize().y / 5));


	//Player 2
	Player* player2 = Instantiate(Player, PlayerTwo);
	assert(player2);

	std::array<KEY, 4> inputKeysPlayer2 =
	{
		KEY::Up,
		KEY::Left,
		KEY::Down,
		KEY::Right,
	};
	player2->SetMovementKeys(inputKeysPlayer2);
	player2->GetTransform().translate(sf::Vector2f(window.getSize().x - window.getSize().x / 5, window.getSize().y / 5));
}
