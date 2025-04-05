#include "SpriteComponent.h"
#include "../../RenderSystem.h"
#include "../../Engine.h"

SpriteComponent::SpriteComponent(Entity* owner, int renderLayer)
{
	std::string type = typeid(this).name();
	m_name = type;

	m_ownerEntity = owner;	
	m_renderLayer = renderLayer;
	m_renderSystem = &Engine::GetInstance()->GetRenderSystem();

	m_drawable = std::make_shared<sf::Sprite>();

	m_renderSystem->AddEntry(m_drawable.get(), this, EntryType::SpriteEntry, m_renderLayer);
	m_hasTexture = true;
}

SpriteComponent::SpriteComponent(std::string& texturePath, Entity* owner, int renderLayer)
{
	std::string type = typeid(this).name();
	m_name = type;

	m_ownerEntity = owner;		
	m_renderLayer = renderLayer;
	m_renderSystem = &Engine::GetInstance()->GetRenderSystem();

	m_drawable = std::make_shared<sf::Sprite>();

	SetTexture(texturePath);
}

void SpriteComponent::SetTexture(std::string& texturePath)
{
	ResourceManager& resourceManager = Engine::GetInstance()->GetResourceManager();

	if (m_hasTexture)
	{
		m_renderSystem->RemoveEntry(this, EntryType::SpriteEntry);
	}
	
	this->m_texturePath = texturePath;
	m_texture = resourceManager.RegisterTexture(this->m_texturePath);
	m_drawable->setTexture(m_texture);

	CenterTexture();
	m_renderSystem->AddEntry(m_drawable.get(), this, EntryType::SpriteEntry, m_renderLayer);
	m_hasTexture = true;	
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
	m_drawable->setTexture(m_texture);

	CenterTexture();
}

void SpriteComponent::CenterTexture()
{
	sf::Vector2u texSize = GetTextureSize();
	m_drawable->setOrigin(0.5f * texSize.x, 0.5f * texSize.y);
}

void SpriteComponent::SetVisibility(bool value)
{
	if (m_isVisible == value || m_drawable == nullptr)
	{
		return;
	}

	m_isVisible = value;
	
	if (value)
	{
		m_renderSystem->AddEntry(m_drawable.get(), this, EntryType::SpriteEntry, m_renderLayer);
	}
	else
	{
		m_renderSystem->RemoveEntry(this, EntryType::SpriteEntry);
	}
}

void SpriteComponent::SetRenderLayer(int layer)
{
	if (m_renderLayer == layer)
	{
		return;
	}

	m_renderLayer = layer;

	if (m_isVisible)
	{
		m_renderSystem->RemoveEntry(this, EntryType::SpriteEntry);
		m_renderSystem->AddEntry(m_drawable.get(), this, EntryType::SpriteEntry, m_renderLayer);
	}
}

sf::Vector2u SpriteComponent::GetTextureSize()
{
	return m_texture.getSize();
}

void SpriteComponent::ShutDown()
{
	if (m_hasTexture && m_isVisible)
	{
		Engine::GetInstance()->GetRenderSystem().RemoveEntry(this, EntryType::SpriteEntry);
	}
}
