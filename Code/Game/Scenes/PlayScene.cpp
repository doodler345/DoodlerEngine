#include "PlayScene.h"

#include <string>

#include "../../Engine/UI_Elements/Image.h"
#include "../Player.h"

void PlayScene::Init()
{
	m_name = "PlayScene";

	sf::RenderWindow& window = Engine::GetInstance()->GetRenderWindow();

	//Create Background Image
	Image* backgroundImage = Instantiate(Image, BackgroundImage);
	assert(backgroundImage);

	std::string path = "../Resources/bmp/DontDelete/GeneratedMap.bmp";
	backgroundImage->m_spriteComponent->SetTexture(path);
	backgroundImage->GetTransform().translate(window.getSize().x / 2, window.getSize().y / 2);


	//Create Game-Entities
	//Player 1
	Player* player = Instantiate(Player, PlayerOne);
	assert(player);


	//***** make this more readable? eg. with Enum Up/Left/Down/Right
	std::array<KEY, 4> keys =
	{
		KEY::W,
		KEY::A,
		KEY::S,
		KEY::D,
	};
	player->SetMovementKeys(keys);
	player->GetTransform().translate(sf::Vector2f(window.getSize().x / 5, window.getSize().y / 5));


	//Player 2
	Player* player2 = Instantiate(Player, PlayerTwo);
	assert(player2);

	std::array<KEY, 4> movementKeys2 =
	{
		KEY::Up,
		KEY::Left,
		KEY::Down,
		KEY::Right,
	};
	player2->SetMovementKeys(movementKeys2);
	player2->GetTransform().translate(sf::Vector2f(window.getSize().x - window.getSize().x / 5, window.getSize().y / 5));
}
