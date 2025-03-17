#include "Bazooka.h"

#include "GameManager.h"
#include "Scenes/PlayScene.h"

void Bazooka::Fire(sf::Vector2f direction, float strength)
{
	Scene* scene = reinterpret_cast<GameManager*>(Engine::GetInstance()->GetGameManager())->GetCurrentScene();
	BazookaRocket* rocket = scene->Instantiate(BazookaRocket, BazookaRocket);
	rocket->GetTransform().translate(m_owner->GetTransform().transformPoint(0,0));
	rocket->Fire(direction, strength);
}
