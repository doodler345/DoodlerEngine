#pragma once

#include <iostream>

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "BazookaRocket.h"

#include "../../Engine/Scene.h"
#include "../../Engine/Entity/Empty.h"
#include "../../Engine/EntityComponents/SpriteComponent.h"

class Bazooka : public Entity
{
public:
	void EntityInit() override;
	void DestroyDerived() override;
	void Update(float deltaTime) override;

	void PullTrigger();
	void Fire(sf::Vector2f direction);
	void SetOwner(Entity* owner) { m_owner = owner; }

private:
	void UpdateShootStrength(float deltaTime);

	Entity* m_owner = nullptr;
	BazookaRocket* m_rocket = nullptr;
	Scene* m_scene = nullptr;

	Empty* m_shootStrengthIndicatorHolder = nullptr;
	std::shared_ptr<SpriteComponent> m_shootStrengthIndicator = nullptr;

	sf::Sound m_shootSound;
	sf::SoundBuffer m_shootSoundBuffer;

	const float m_SHOOT_STRENGTH_INDICATOR_SCALE = 1.5f;
	const float m_SHOOT_STRENGTH_INDICATOR_XOFFSET = 35.f;

	float m_shootStrength = 0;
	bool m_loadShootStrength = false;
};