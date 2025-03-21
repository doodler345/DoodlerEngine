#include "RenderSystem.h"

#include "Debug.h"
#include "Engine.h"


void RenderSystem::Update()
{
	sf::RenderWindow& renderWindow = Engine::GetInstance()->GetRenderWindow();

	sf::RenderStates renderState;
	for (auto& it : m_shaderEntries)
	{
		renderState.transform = it.second.m_owner->GetOwner()->GetTransformable().getTransform();
		renderState.shader = it.second.m_shader;
		renderWindow.draw(*it.second.m_drawable, renderState); 
	}

	for (auto& it : m_spriteEntries)
	{
		renderWindow.draw(*it.second.m_drawable, it.second.m_owner->GetOwner()->GetTransformable().getTransform());
	}

	for (auto& it : m_shapeEntries)
	{
		renderWindow.draw(*it.second.m_drawable, it.second.m_owner->GetOwner()->GetTransformable().getTransform());
	}

	for (auto& it : m_textEntries)
	{
		renderWindow.draw(*it.second.m_drawable, it.second.m_owner->GetOwner()->GetTransformable().getTransform());
	}
}

void RenderSystem::AddSprite(sf::Sprite* sprite, EntityComponent* owner)
{
	RenderEntry entry;
	entry.m_owner = owner;
	entry.m_drawable = sprite;

	int id = owner->m_id;
	m_spriteEntries[id] = entry;

	DebugPrint("Added Sprite to EntityComponent (c_id " + std::to_string(owner->m_id) + ")", TextColor::Yellow, DebugChannel::RenderSystem, __FILE__, __LINE__);
}

void RenderSystem::AddText(sf::Text* text, EntityComponent* owner)
{
	RenderEntry entry;
	entry.m_owner = owner;
	entry.m_drawable = text;
	m_textEntries[owner->m_id] = entry;

	DebugPrint("Added Text to EntityComponent (c_id " + std::to_string(owner->m_id) + ")", TextColor::Yellow, DebugChannel::RenderSystem, __FILE__, __LINE__);
}

void RenderSystem::AddShape(sf::Shape* drawable, EntityComponent* owner)
{
	RenderEntry entry;
	entry.m_owner = owner;
	entry.m_drawable = drawable;
	m_shapeEntries[owner->m_id] = entry;

	DebugPrint("Added Shape to EntityComponent (c_id " + std::to_string(owner->m_id) + ")", TextColor::Yellow, DebugChannel::RenderSystem, __FILE__, __LINE__);
}

void RenderSystem::AddShader(sf::Drawable* drawable, sf::Shader* shader, EntityComponent* owner)
{
	RenderEntry entry;
	entry.m_owner = owner;
	entry.m_drawable = drawable;
	entry.m_shader = shader;
	m_shaderEntries[owner->m_id] = entry;

	DebugPrint("Added Shader to EntityComponent (c_id " + std::to_string(owner->m_id) + ")", TextColor::Yellow, DebugChannel::RenderSystem, __FILE__, __LINE__);
}

void RenderSystem::RemoveSprite(EntityComponent* owner)
{
	if (auto search = m_spriteEntries.find(owner->m_id); search != m_spriteEntries.end())
	{
		m_spriteEntries.erase(search);
		DebugPrint("Removed Sprite from EntityComponent (c_id " + std::to_string(owner->m_id) + ")", TextColor::Yellow, DebugChannel::RenderSystem, __FILE__, __LINE__);
	}
	else
	{
		DebugPrint("Couldn't remove Sprite from EntityComponent (c_id " + std::to_string(owner->m_id) + ")", TextColor::Red, DebugChannel::RenderSystem, __FILE__, __LINE__, 1);
	}
}

void RenderSystem::RemoveText(EntityComponent* owner)
{
	if (auto search = m_textEntries.find(owner->m_id); search != m_textEntries.end())
	{
		m_textEntries.erase(search);
		DebugPrint("Removed Text from EntityComponent (c_id " + std::to_string(owner->m_id) + ")", TextColor::Yellow, DebugChannel::RenderSystem, __FILE__, __LINE__);
	}
	else
	{
		DebugPrint("Couldn't remove Text from EntityComponent (c_id " + std::to_string(owner->m_id) + ")", TextColor::Red, DebugChannel::RenderSystem, __FILE__, __LINE__, 1);
	}
}

void RenderSystem::RemoveShape(EntityComponent* owner)
{
	if (auto search = m_shapeEntries.find(owner->m_id); search != m_shapeEntries.end())
	{
		m_shapeEntries.erase(search);
		DebugPrint("Removed Shape from EntityComponent (c_id " + std::to_string(owner->m_id) + ")", TextColor::Yellow, DebugChannel::RenderSystem, __FILE__, __LINE__);
	}
	else
	{
		DebugPrint("Couldn't remove Shape from EntityComponent (c_id " + std::to_string(owner->m_id) + ")", TextColor::Red, DebugChannel::RenderSystem, __FILE__, __LINE__, 1);
	}
}

void RenderSystem::RemoveShader(EntityComponent* owner)
{
	if (auto search = m_shaderEntries.find(owner->m_id); search != m_shaderEntries.end())
	{
		m_shaderEntries.erase(search);
		DebugPrint("Removed Shader from EntityComponent (c_id " + std::to_string(owner->m_id) + ")", TextColor::Yellow, DebugChannel::RenderSystem, __FILE__, __LINE__);
	}
	else
	{
		DebugPrint("Couldn't remove Shader from EntityComponent (c_id " + std::to_string(owner->m_id) + ")", TextColor::Red, DebugChannel::RenderSystem, __FILE__, __LINE__, 1);
	}
}
