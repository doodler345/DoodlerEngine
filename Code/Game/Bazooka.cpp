#include "Bazooka.h"

#include "GameManager.h"
#include "Scenes/PlayScene.h"

void Bazooka::EntityInit()
{
	m_shootSoundBuffer.loadFromFile("../Resources/Audio/Aud_Sound_Pistol_0.wav");
}

void Bazooka::DestroyDerived()
{
}

void Bazooka::Update(float deltaTime)
{
	if (m_loadShootStrength)
	{
		m_shootStrength += deltaTime * 1000;
	}
}

void Bazooka::PullTrigger()
{
	m_loadShootStrength = true;
	m_shootStrength = 0;
}

void Bazooka::Fire(sf::Vector2f direction)
{
	Scene* scene = reinterpret_cast<GameManager*>(Engine::GetInstance()->GetGameManager())->GetCurrentScene();
	BazookaRocket* rocket = scene->Instantiate(BazookaRocket, BazookaRocket);
	rocket->GetTransformable().move(m_owner->GetTransformable().getPosition());
	rocket->Fire(direction, m_shootStrength);
	
	m_shootSound = sf::Sound(m_shootSoundBuffer);
	m_shootSound.play();

	m_loadShootStrength = false;
}