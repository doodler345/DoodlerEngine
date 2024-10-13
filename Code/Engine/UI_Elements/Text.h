#pragma once

#include <memory>

#include "../Entity/Entity.h"
#include "../EntityComponents/TextComponent.h"


class Text : public Entity
{
public:
	TextComponent* m_textComponent;

private:
	void EntityInit() override;
	void DestroyDerived() {};

};

