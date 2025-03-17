#include "BazookaRocket.h"

#include "GameManager.h"
#include "Scenes/PlayScene.h"
#include "../../Engine/EntityComponents/SpriteComponent.h"

void BazookaRocket::EntityInit()
{
	DebugPrint("BazookaRocket " + std::to_string(m_id) + " spawned", TextColor::Green, DebugChannel::Entity, __FILE__, __LINE__);

	std::string path = "../Resources/Profilbild Sonic Infusion.png";
	std::shared_ptr<SpriteComponent> spriteComponent = std::make_shared<SpriteComponent>(path, this);
	spriteComponent->m_drawable.setScale(m_SPRITE_SCALE, m_SPRITE_SCALE);
	m_spriteSize = spriteComponent->m_drawable.getGlobalBounds().getSize();
	AddComponent(spriteComponent);

	GameManager* gameManager = reinterpret_cast<GameManager*>(Engine::GetInstance()->GetGameManager());
	m_world = reinterpret_cast<PlayScene*>(gameManager->GetCurrentScene())->GetWorld();
	assert(m_world);

	GetTransform().translate(sf::Vector2f(50, 0));
}

void BazookaRocket::DestroyDerived()
{
}
