#include "Bazooka.h"

#include "GameManager.h"
#include "Scenes/PlayScene.h"

void Bazooka::EntityInit()
{
	m_shootSoundBuffer.loadFromFile("../Resources/Audio/Aud_Sound_BazookaShot_0.wav");

	std::string path = "../Resources/Sprites/ShootStrengthIndicator.png";
	m_scene = reinterpret_cast<GameManager*>(Engine::GetInstance()->GetGameManagerEntity())->GetCurrentScene();
	m_shootStrengthIndicatorHolder = m_scene->Instantiate(Empty, ShootStrengthIndicatorHolder);
	m_shootStrengthIndicatorHolder->SetParent(this);
	m_shootStrengthIndicator = std::make_shared<SpriteComponent>(path, m_shootStrengthIndicatorHolder);
	m_shootStrengthIndicator->m_drawable->setPosition(m_SHOOT_STRENGTH_INDICATOR_XOFFSET, 0);
	m_shootStrengthIndicator->m_drawable->setScale(0, m_SHOOT_STRENGTH_INDICATOR_SCALE);

	m_shootStrengthIndicatorHolder->AddComponent(m_shootStrengthIndicator);
}

void Bazooka::DestroyDerived()
{
}

void Bazooka::Update(float deltaTime)
{
	UpdateShootStrength(deltaTime);
}

void Bazooka::PullTrigger()
{
	m_loadShootStrength = true;
	m_shootStrength = 0;
}

void Bazooka::Fire(sf::Vector2f direction)
{
	BazookaRocket* rocket = m_scene->Instantiate(BazookaRocket, BazookaRocket);
	rocket->GetTransformable().move(m_owner->GetTransformable().getPosition());
	rocket->Fire(direction, m_shootStrength);
	
	m_shootSound = sf::Sound(m_shootSoundBuffer);
	m_shootSound.play();

	m_loadShootStrength = false;
	m_shootStrengthIndicator->m_drawable->setScale(0, m_SHOOT_STRENGTH_INDICATOR_SCALE);
}

void Bazooka::UpdateShootStrength(float deltaTime)
{
	if (m_loadShootStrength)
	{
		m_shootStrength += deltaTime * 1000;
		m_shootStrengthIndicator->m_drawable->setScale(m_shootStrength * 0.05f, m_SHOOT_STRENGTH_INDICATOR_SCALE);
	}
}
