#include "SpriteComponent.h"
#include "../RenderSystem.h"
#include "../Engine.h"

SpriteComponent::SpriteComponent(Entity* owner)
{
	m_ownerEntity = owner;		
}

SpriteComponent::SpriteComponent(std::string& texturePath, Entity* owner)
{
	m_ownerEntity = owner;		
	SetTexture(texturePath);
}

void SpriteComponent::SetTexture(std::string& texturePath)
{
	RenderSystem& renderSystem = Engine::GetInstance()->GetRenderSystem();
	ResourceManager& resourceManager = Engine::GetInstance()->GetResourceManager();

	if (m_isInitialized)
	{
		renderSystem.RemoveSprite(m_ownerEntity);
	}
	else
	{
		m_isInitialized = true;
	}
	
	this->m_texturePath = texturePath;
	m_texture = resourceManager.RegisterTexture(this->m_texturePath);
	m_drawable.setTexture(m_texture);

	CenterTexture();

	renderSystem.AddSprite(&m_drawable, m_ownerEntity);
}

void SpriteComponent::UpdateTexture()
{
	ResourceManager& resourceManager = Engine::GetInstance()->GetResourceManager();
	if (m_texturePath == "")
	{
		DebugPrint("texturePath not set, cant update", TextColor::Red, DebugChannel::EntityComponent, __FILE__, __LINE__, 1);
		return;
	}
	m_texture = resourceManager.UpdateTexture(m_texturePath);
	m_drawable.setTexture(m_texture);

	CenterTexture();
}

void SpriteComponent::CenterTexture()
{
	sf::Vector2u texSize = GetTextureSize();
	m_drawable.setOrigin(0.5f * texSize.x, 0.5f * texSize.y);
}

sf::Vector2u SpriteComponent::GetTextureSize()
{
	return m_texture.getSize();
}

void SpriteComponent::ShutDown()
{
	if (m_texturePath != "")
	{
		Engine::GetInstance()->GetRenderSystem().RemoveSprite(m_ownerEntity);
	}
}
