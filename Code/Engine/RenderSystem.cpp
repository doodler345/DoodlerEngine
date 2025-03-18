#include "RenderSystem.h"

#include "Debug.h"
#include "Engine.h"


void RenderSystem::Update()
{
	sf::RenderWindow& renderWindow = Engine::GetInstance()->GetRenderWindow();

	sf::RenderStates renderState;
	for (int i = 0; i < m_shaderEntries.size(); i++)
	{
		renderState.transform = m_shaderEntries[i].m_owner->GetTransformable().getTransform();
		renderState.shader = m_shaderEntries[i].m_shader;
		renderWindow.draw(*m_shaderEntries[i].m_drawable, renderState); // ***** Transform missing (maybe)?
	}

	for (int i = 0; i < m_spriteEntries.size(); i++)
	{
		renderWindow.draw(*m_spriteEntries[i].m_drawable, m_spriteEntries[i].m_owner->GetTransformable().getTransform());
	}

	for (int i = 0; i < m_shapeEntries.size(); i++)
	{
		renderWindow.draw(*m_shapeEntries[i].m_drawable, m_shapeEntries[i].m_owner->GetTransformable().getTransform());
	}

	for (int i = 0; i < m_textEntries.size(); i++)
	{
		renderWindow.draw(*m_textEntries[i].m_drawable, m_textEntries[i].m_owner->GetTransformable().getTransform());
	}


}

void RenderSystem::AddSprite(sf::Sprite* sprite, Entity* owner)
{
	RenderEntry entry;
	entry.m_owner = owner;
	entry.m_drawable = sprite;
	m_spriteEntries.push_back(entry);
}

void RenderSystem::RemoveSprite(Entity* owner)
{
	for (int i = 0; i < m_spriteEntries.size(); i++)
	{
		if (owner->GetID() == m_spriteEntries[i].m_owner->GetID())
		{
			m_spriteEntries.erase(m_spriteEntries.begin() + i);
			DebugPrint("Removed Sprite from Entity " + owner->GetName() + " " + std::to_string(owner->GetID()), TextColor::Yellow, DebugChannel::RenderSystem, __FILE__, __LINE__);
			return;
		}
	}

	DebugPrint("Couldn't remove Sprite from Entity " + owner->GetName() + " " + std::to_string(owner->GetID()), TextColor::Red, DebugChannel::RenderSystem, __FILE__, __LINE__, 1);
}

void RenderSystem::AddText(sf::Text* text, Entity* owner)
{
	RenderEntry entry;
	entry.m_owner = owner;
	entry.m_drawable = text;
	m_textEntries.push_back(entry);
}

void RenderSystem::RemoveText(Entity* owner)
{
	for (int i = 0; i < m_textEntries.size(); i++)
	{
		if (owner->GetID() == m_textEntries[i].m_owner->GetID())
		{
			m_textEntries.erase(m_textEntries.begin() + i);
			DebugPrint("Removed Text from Entity " + owner->GetName() + " " + std::to_string(owner->GetID()), TextColor::Yellow, DebugChannel::RenderSystem, __FILE__, __LINE__);
			return;
		}
	}

	DebugPrint("Couldn't remove Text from Entity " + owner->GetName() + " " + std::to_string(owner->GetID()), TextColor::Red, DebugChannel::RenderSystem, __FILE__, __LINE__, 1);
}

void RenderSystem::AddShape(sf::Shape* drawable, Entity* owner)
{
	RenderEntry entry;
	entry.m_owner = owner;
	entry.m_drawable = drawable;
	m_shapeEntries.push_back(entry);
}

void RenderSystem::RemoveShape(Entity* owner)
{
	for (int i = 0; i < m_shapeEntries.size(); i++)
	{
		if (owner->GetID() == m_shapeEntries[i].m_owner->GetID())
		{
			m_shapeEntries.erase(m_shapeEntries.begin() + i);
			DebugPrint("Removed Shape from Entity " + owner->GetName() + " " + std::to_string(owner->GetID()), TextColor::Yellow, DebugChannel::RenderSystem, __FILE__, __LINE__);
			return;
		}
	}

	DebugPrint("Couldn't remove Shape from Entity " + owner->GetName() + " " + std::to_string(owner->GetID()), TextColor::Red, DebugChannel::RenderSystem, __FILE__, __LINE__, 1);
}

void RenderSystem::AddShader(sf::Drawable* drawable, sf::Shader* shader, Entity* owner)
{
	RenderEntry entry;
	entry.m_owner = owner;
	entry.m_drawable = drawable;
	entry.m_shader = shader;
	m_shaderEntries.push_back(entry);
}

void RenderSystem::RemoveShader(Entity* owner)
{
	for (int i = 0; i < m_shaderEntries.size(); i++)
	{
		if (owner->GetID() == m_shaderEntries[i].m_owner->GetID())
		{
			m_shaderEntries.erase(m_shaderEntries.begin() + i);
			DebugPrint("Removed Shader from Entity " + owner->GetName() + " " + std::to_string(owner->GetID()), TextColor::Yellow, DebugChannel::RenderSystem, __FILE__, __LINE__);
			return;
		}
	}

	DebugPrint("Couldn't remove Shader from Entity " + owner->GetName() + " " + std::to_string(owner->GetID()), TextColor::Red, DebugChannel::RenderSystem, __FILE__, __LINE__, 1);
}
