#include "Bazooka.h"

#include "GameManager.h"
#include "Scenes/PlayScene.h"

void Bazooka::Fire(sf::Vector2f direction, float strength)
{
	std::cout << "Fire!" << std::endl;

	Scene* scene = reinterpret_cast<GameManager*>(Engine::GetInstance()->GetGameManager())->GetCurrentScene();
	scene->Instantiate(BazookaRocket, BazookaRocket);
}
