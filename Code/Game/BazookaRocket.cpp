#include "BazookaRocket.h"

#include "GameManager.h"
#include "Scenes/PlayScene.h"
#include "../../Engine/EntityComponents/SpriteComponent.h"

void BazookaRocket::EntityInit()
{
	DebugPrint("BazookaRocket " + std::to_string(m_id) + " spawned", TextColor::Green, DebugChannel::Entity, __FILE__, __LINE__);

	std::string path = "../Resources/BazookaRocket.png";
	m_spriteComponent = std::make_shared<SpriteComponent>(path, this);
	m_spriteComponent->m_drawable.setScale(m_SPRITE_SCALE, m_SPRITE_SCALE);
	m_spriteSize = m_spriteComponent->m_drawable.getGlobalBounds().getSize();
	AddComponent(m_spriteComponent);

	GameManager* gameManager = reinterpret_cast<GameManager*>(Engine::GetInstance()->GetGameManager());
	m_world = reinterpret_cast<PlayScene*>(gameManager->GetCurrentScene())->GetWorld();
	assert(m_world);
}

void BazookaRocket::Fire(sf::Vector2f direction, float strength)
{
	m_velocity = direction * strength;
}

void BazookaRocket::Update(float deltaTime)
{
	m_velocity += sf::Vector2f(0, 800) * deltaTime; // Applying fake gravity
	GetTransform().translate(m_velocity * deltaTime);

	m_spriteComponent->m_drawable.setRotation(atan2(m_velocity.y, m_velocity.x) * 180 / 3.14159265f);

	// TODO: Check if at window bounds and destroy

	int pixelCheck = *m_world->GetPixelValue(m_world->WorldToScreenPosition(sf::Vector2u(GetTransform().transformPoint(0,0))));
	if (pixelCheck == 1)
	{
		Explode();
	}
}

void BazookaRocket::Explode()
{
	Scene* scene = reinterpret_cast<GameManager*>(Engine::GetInstance()->GetGameManager())->GetCurrentScene();
	scene->DestroyEntity(this);
}

void BazookaRocket::DestroyDerived()
{
}
