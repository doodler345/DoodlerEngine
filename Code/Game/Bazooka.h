#include <iostream>

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "BazookaRocket.h"

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
	Entity* m_owner = nullptr;
	BazookaRocket* m_rocket = nullptr;

	sf::Sound m_shootSound;
	sf::SoundBuffer m_shootSoundBuffer;
	float m_shootStrength = 0;
	bool m_loadShootStrength = false;
};