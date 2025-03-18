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
	GetTransformable().move(m_velocity * deltaTime);

	m_spriteComponent->m_drawable.setRotation(atan2(m_velocity.y, m_velocity.x) * 180 / std::numbers::pi);
}

void BazookaRocket::CheckCollision()
{
	sf::Vector2i worldPosition = sf::Vector2i(GetTransformable().getPosition());

	// Check if window ground collision
	sf::Vector2i screenPosition = m_world->WorldToScreenPosition(sf::Vector2u(worldPosition));
	if (worldPosition.y >= m_world->m_worldHeight) 
	{
		Explode(screenPosition);
		return;
	}

	
	// TODO: Make a real Radius Collision check instead of an AXIS-Collison check
	// Check if world collision
	// horizontal axis

	sf::Vector2u windowSize = Engine::GetInstance()->GetRenderWindow().getSize();

	for (int i = 0; i < m_spriteSizeWorld.x * m_COLLISION_RADIUS_RELATIVE; i++)
	{
		sf::Vector2i xOffset = sf::Vector2i(0.5f * (m_spriteSizeWorld.x * m_COLLISION_RADIUS_RELATIVE), 0);
		screenPosition = m_world->WorldToScreenPosition(sf::Vector2u(worldPosition - xOffset + sf::Vector2i(i, 0)));

		if (screenPosition.x >= windowSize.x || screenPosition.x < 0 || screenPosition.y >= windowSize.y || screenPosition.y < 0)
		{
			continue;
		}

		int pixelCheck = *m_world->GetPixelValue(sf::Vector2u(screenPosition));
		if (pixelCheck == 1)
		{
			Explode(screenPosition);
			return;
		}
	}
	// vertical axis
	for (int i = 0; i < m_spriteSizeWorld.y * m_COLLISION_RADIUS_RELATIVE; i++)
	{
		sf::Vector2i yOffset = sf::Vector2i(0, 0.5f * (m_spriteSizeWorld.y * m_COLLISION_RADIUS_RELATIVE));
		screenPosition = m_world->WorldToScreenPosition(sf::Vector2u(worldPosition - yOffset + sf::Vector2i(0, i)));

		if (screenPosition.x >= windowSize.x || screenPosition.x < 0 || screenPosition.y >= windowSize.y || screenPosition.y < 0)
		{
			continue;
		}

		int pixelCheck = *m_world->GetPixelValue(sf::Vector2u(screenPosition));
		if (pixelCheck == 1)
		{
			Explode(screenPosition);
			return;
		}
	}
}

void BazookaRocket::Explode(sf::Vector2i worldPosition)
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
