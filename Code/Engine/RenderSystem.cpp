#include "RenderSystem.h"

#include "Debug.h"
#include "Engine.h"


void RenderSystem::Update()
{
	sf::RenderWindow& renderWindow = Engine::GetInstance()->GetRenderWindow();

	sf::RenderStates renderState;
	for (auto& it : m_shaderEntries)
	{
		renderState.transform = it.second.m_owner->GetTransformable().getTransform();
		renderState.shader = it.second.m_shader;
		renderWindow.draw(*it.second.m_drawable, renderState); // ***** Transform missing (maybe)?
	}

	for (auto& it : m_spriteEntries)
	{
		renderWindow.draw(*it.second.m_drawable, it.second.m_owner->GetTransformable().getTransform());
	}

	for (auto& it : m_shapeEntries)
	{
		renderWindow.draw(*it.second.m_drawable, it.second.m_owner->GetTransformable().getTransform());
	}

	for (auto& it : m_textEntries)
	{
		renderWindow.draw(*it.second.m_drawable, it.second.m_owner->GetTransformable().getTransform());
	}
}

void RenderSystem::AddSprite(sf::Sprite* sprite, Entity* owner)
{
	RenderEntry entry;
	entry.m_owner = owner;
	entry.m_drawable = sprite;

	int id = owner->GetID();
	m_spriteEntries[id] = entry;
}

void RenderSystem::AddText(sf::Text* text, Entity* owner)
{
	RenderEntry entry;
	entry.m_owner = owner;
	entry.m_drawable = text;
	m_textEntries[owner->GetID()] = entry;
}

void RenderSystem::AddShape(sf::Shape* drawable, Entity* owner)
{
	RenderEntry entry;
	entry.m_owner = owner;
	entry.m_drawable = drawable;
	m_shapeEntries[owner->GetID()] = entry;
}

void RenderSystem::AddShader(sf::Drawable* drawable, sf::Shader* shader, Entity* owner)
{
	RenderEntry entry;
	entry.m_owner = owner;
	entry.m_drawable = drawable;
	entry.m_shader = shader;
	m_shaderEntries[owner->GetID()] = entry;
}

void RenderSystem::RemoveSprite(Entity* owner)
{
	if (auto search = m_spriteEntries.find(owner->GetID()); search != m_spriteEntries.end())
	{
		m_spriteEntries.erase(search);
		DebugPrint("Removed Sprite from Entity " + owner->GetName() + " " + std::to_string(owner->GetID()), TextColor::Yellow, DebugChannel::RenderSystem, __FILE__, __LINE__);
	}
	else
	{
		DebugPrint("Couldn't remove Sprite from Entity " + owner->GetName() + " " + std::to_string(owner->GetID()), TextColor::Red, DebugChannel::RenderSystem, __FILE__, __LINE__, 1);
	}
}

void RenderSystem::RemoveText(Entity* owner)
{
	if (auto search = m_textEntries.find(owner->GetID()); search != m_textEntries.end())
	{
		m_textEntries.erase(search);
		DebugPrint("Removed Text from Entity " + owner->GetName() + " " + std::to_string(owner->GetID()), TextColor::Yellow, DebugChannel::RenderSystem, __FILE__, __LINE__);
	}
	else
	{
		DebugPrint("Couldn't remove Text from Entity " + owner->GetName() + " " + std::to_string(owner->GetID()), TextColor::Red, DebugChannel::RenderSystem, __FILE__, __LINE__, 1);
	}
}

void RenderSystem::RemoveShape(Entity* owner)
{
	if (auto search = m_shapeEntries.find(owner->GetID()); search != m_shapeEntries.end())
	{
		m_shapeEntries.erase(search);
		DebugPrint("Removed Shape from Entity " + owner->GetName() + " " + std::to_string(owner->GetID()), TextColor::Yellow, DebugChannel::RenderSystem, __FILE__, __LINE__);
	}
	else
	{
		DebugPrint("Couldn't remove Shape from Entity " + owner->GetName() + " " + std::to_string(owner->GetID()), TextColor::Red, DebugChannel::RenderSystem, __FILE__, __LINE__, 1);
	}
}

void RenderSystem::RemoveShader(Entity* owner)
{
	if (auto search = m_shaderEntries.find(owner->GetID()); search != m_shaderEntries.end())
	{
		m_shaderEntries.erase(search);
		DebugPrint("Removed Shader from Entity " + owner->GetName() + " " + std::to_string(owner->GetID()), TextColor::Yellow, DebugChannel::RenderSystem, __FILE__, __LINE__);
	}
	else
	{
		DebugPrint("Couldn't remove Shader from Entity " + owner->GetName() + " " + std::to_string(owner->GetID()), TextColor::Red, DebugChannel::RenderSystem, __FILE__, __LINE__, 1);
	}
}
