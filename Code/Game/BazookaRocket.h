#include "../Engine/Engine.h"
#include "../Engine/Entity/Entity.h"
#include "../Engine/EntityComponents/SpriteComponent.h"
#include "World.h"

class BazookaRocket : public Entity
{
public:
	void EntityInit() override;
	void Fire(sf::Vector2f direction, float strength);
protected:
	void Update(float deltaTime) override;

	void Explode(sf::Vector2u worldPosition);

	void DestroyDerived() override;

private: 
	const float m_SPRITE_SCALE = 0.75f;
	const int m_EXPLOSION_RADIUS = 50;

	World* m_world = nullptr;
	std::shared_ptr<SpriteComponent> m_spriteComponent = nullptr;
	sf::Vector2f m_spriteSize;
	sf::Vector2f m_shootForce;
	sf::Vector2f m_velocity;

	float timer = 0;
};