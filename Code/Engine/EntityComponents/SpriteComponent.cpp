#include "SpriteComponent.h"
#include "../RenderSystem.h"
#include "../Engine.h"

SpriteComponent::SpriteComponent(Entity* owner, int renderLayer)
{
	std::string type = typeid(this).name();
	m_name = type;

	m_ownerEntity = owner;	
	m_renderLayer = renderLayer;
	m_renderSystem = &Engine::GetInstance()->GetRenderSystem();
}

SpriteComponent::SpriteComponent(std::string& texturePath, Entity* owner, int renderLayer)
{
	std::string type = typeid(this).name();
	m_name = type;

	m_ownerEntity = owner;		
	m_renderLayer = renderLayer;
	m_renderSystem = &Engine::GetInstance()->GetRenderSystem();

	SetTexture(texturePath);
}

void SpriteComponent::SetTexture(std::string& texturePath)
{
	RenderSystem& renderSystem = Engine::GetInstance()->GetRenderSystem();
	ResourceManager& resourceManager = Engine::GetInstance()->GetResourceManager();

	if (m_isInitialized)
	{
		renderSystem.RemoveEntry(this, EntryType::SpriteEntry);
	}
	else
	{
		m_isInitialized = true;
	}
	
	this->m_texturePath = texturePath;
	m_texture = resourceManager.RegisterTexture(this->m_texturePath);
	m_drawable.setTexture(m_texture);

	CenterTexture();

	renderSystem.AddEntry(&m_drawable, this, EntryType::SpriteEntry, m_renderLayer);
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

void SpriteComponent::SetVisibility(bool value)
{
	if (m_isVisible == value)
	{
		return;
	}

	m_isVisible = value;
	
	if (value)
	{
		m_renderSystem->AddEntry(&m_drawable, this, EntryType::SpriteEntry, m_renderLayer);
	}
	else
	{
		m_renderSystem->RemoveEntry(this, EntryType::SpriteEntry);
	}
}

sf::Vector2u SpriteComponent::GetTextureSize()
{
	return m_texture.getSize();
}

void SpriteComponent::ShutDown()
{
	if (m_texturePath != "")
	{
		Engine::GetInstance()->GetRenderSystem().RemoveEntry(this, EntryType::SpriteEntry);
	}
}
