#include "../Engine/Engine.h"
#include "../Engine/Entity/Entity.h"
#include "World.h"

class BazookaRocket : public Entity
{
public:
	void EntityInit() override;
protected:
	void DestroyDerived() override;

private: 
	const float m_SPRITE_SCALE = 1.5f;

	World* m_world = nullptr;
	sf::Vector2f m_spriteSize;
};