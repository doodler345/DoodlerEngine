#pragma once

#include "../Entity/Entity.h"
#include "../EntityComponents/SpriteComponent.h"

class Image : public Entity
{
public:
	void EntityInit() override;
	std::shared_ptr<SpriteComponent> m_spriteComponent;
private:
	void DestroyDerived() override;
};

