#include "Image.h"

#include <memory>
#include <string>

void Image::EntityInit()
{
	m_spriteComponent = std::make_shared<SpriteComponent>(this);
	AddComponent(m_spriteComponent);
}

void Image::DestroyDerived()
{
	//***** 
}
