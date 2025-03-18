#include "BazookaRocket.h"

#include <numbers>

#include "GameManager.h"
#include "Scenes/PlayScene.h"
#include "../../Engine/EntityComponents/SpriteComponent.h"

void BazookaRocket::EntityInit()
{
	DebugPrint("BazookaRocket " + std::to_string(m_id) + " spawned", TextColor::Green, DebugChannel::Entity, __FILE__, __LINE__);

	std::string path = "../Resources/Sprites/BazookaRocket.png";
	m_spriteComponent = std::make_shared<SpriteComponent>(path, this);
	m_spriteComponent->m_drawable.setScale(m_SPRITE_SCALE, m_SPRITE_SCALE);
	m_spriteSize = m_spriteComponent->m_drawable.getGlobalBounds().getSize();
	AddComponent(m_spriteComponent);

	GameManager* gameManager = reinterpret_cast<GameManager*>(Engine::GetInstance()->GetGameManager());
	m_world = reinterpret_cast<PlayScene*>(gameManager->GetCurrentScene())->GetWorld();
	assert(m_world);
	
	m_spriteSizeWorld = m_world->ScreenToWorldPosition(sf::Vector2u(m_spriteSize));
}

void BazookaRocket::Fire(sf::Vector2f direction, float strength)
{
	m_velocity = direction * strength;
}

void BazookaRocket::Update(float deltaTime)
{
	Move(deltaTime);
	CheckCollision();
}

void BazookaRocket::Move(float deltaTime)
{
	m_velocity += sf::Vector2f(0, 800) * deltaTime; // Applying fake gravity
	GetTransform().translate(m_velocity * deltaTime);

	m_spriteComponent->m_drawable.setRotation(atan2(m_velocity.y, m_velocity.x) * 180 / std::numbers::pi);
}

void BazookaRocket::CheckCollision()
{
	// Check if window ground collision
	sf::Vector2u worldPosition = sf::Vector2u(GetTransform().transformPoint(0, 0));
	sf::Vector2u screenPosition = m_world->WorldToScreenPosition(worldPosition);
	if (worldPosition.y >= m_world->m_worldHeight)
	{
		Explode(screenPosition);
		return;
	}

	
	// TODO: Make a real Radius Collision Check instead of an AXIS-Collison Check
	// Check if world collision
	// horizontal axis
	for (int i = 0; i < m_spriteSizeWorld.x * m_COLLISION_RADIUS_RELATIVE; i++)
	{
		sf::Vector2u xOffset = sf::Vector2u(0.5f * (m_spriteSizeWorld.x * m_COLLISION_RADIUS_RELATIVE), 0);
		screenPosition = m_world->WorldToScreenPosition(worldPosition - xOffset + sf::Vector2u(i, 0));
		int pixelCheck = *m_world->GetPixelValue(screenPosition);
		if (pixelCheck == 1)
		{
			Explode(screenPosition);
			return;
		}
	}
	// vertical axis
	for (int i = 0; i < m_spriteSizeWorld.y * m_COLLISION_RADIUS_RELATIVE; i++)
	{
		sf::Vector2u yOffset = sf::Vector2u(0, 0.5f * (m_spriteSizeWorld.y * m_COLLISION_RADIUS_RELATIVE));
		screenPosition = m_world->WorldToScreenPosition(worldPosition - yOffset + sf::Vector2u(0, i));
		int pixelCheck = *m_world->GetPixelValue(screenPosition);
		if (pixelCheck == 1)
		{
			Explode(screenPosition);
			return;
		}
	}
}

void BazookaRocket::Explode(sf::Vector2u worldPosition)
{
	int x;
	int prevX = worldPosition.x + cos(0) * m_EXPLOSION_RADIUS * -1;
	int deltaX;
	for (int i = -m_EXPLOSION_RADIUS; i < m_EXPLOSION_RADIUS; i++)
	{
		int xOffset = cos((i + m_EXPLOSION_RADIUS) / (2.0f * m_EXPLOSION_RADIUS) * std::numbers::pi) * m_EXPLOSION_RADIUS * -1;
		int yMaxOffset = sin((i + m_EXPLOSION_RADIUS) / (2.0f * m_EXPLOSION_RADIUS) * std::numbers::pi) * m_EXPLOSION_RADIUS;

		x = worldPosition.x + xOffset;
		deltaX = x - prevX;

		// Sometimes deltaX is 0 or 2, because of the Cosine. That can lead to unnecessary calculations, or worse, horizontal gaps 
		// With substeps this cant happen anymore
		for (int xSubsteps = 1; xSubsteps < deltaX + 1; xSubsteps++)
		{
			for (int yOffset = -yMaxOffset; yOffset < yMaxOffset; yOffset++)
			{
				int y = worldPosition.y + yOffset;
				if (x >= m_world->m_worldWidth || x < 0 || y >= m_world->m_worldHeight || y < 0)
				{
					continue;
				}

				int pixelIndex = y * m_world->m_worldWidth + prevX + xSubsteps;

				m_world->TryDestroyPixel(pixelIndex);
			}
		}


		prevX = x;
	}

	m_world->UpdateTexture();


	Scene* scene = reinterpret_cast<GameManager*>(Engine::GetInstance()->GetGameManager())->GetCurrentScene();
	scene->DestroyEntity(this);
}

void BazookaRocket::DestroyDerived()
{
}
