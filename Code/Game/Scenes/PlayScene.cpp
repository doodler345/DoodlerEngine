#include "PlayScene.h"

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

	world->GetTransform().translate(window.getSize().x / 2, window.getSize().y / 2);
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

	std::array<KEY, 2> inputKeysPlayer1 =
	{
		KEY::A,
		KEY::D,
	};
	player->SetMovementKeys(inputKeysPlayer1);
	player->GetTransform().translate(sf::Vector2f(window.getSize().x / 5, window.getSize().y / 5));


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
