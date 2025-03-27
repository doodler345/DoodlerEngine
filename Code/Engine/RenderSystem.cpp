#include "RenderSystem.h"

#include "Debug.h"
#include "Engine.h"


void RenderSystem::Init(sf::RenderWindow* renderWindow)
{
	m_renderWindow = renderWindow;
}

void RenderSystem::Update()
{

	for (auto& it : m_renderLayers)
	{
		sf::RenderStates renderState;
		for (auto& it2 : it.second->m_shaderEntries)
		{
			renderState.transform = it2.second.m_owner->GetOwner()->GetTransformable().getTransform();
			renderState.shader = it2.second.m_shader;
			m_renderWindow->draw(*it2.second.m_drawable, renderState);
		}

		for (auto& it2 : it.second->m_spriteEntries)
		{
			m_renderWindow->draw(*it2.second.m_drawable, it2.second.m_owner->GetOwner()->GetTransformable().getTransform());
		}

		for (auto& it2 : it.second->m_shapeEntries)
		{
			m_renderWindow->draw(*it2.second.m_drawable, it2.second.m_owner->GetOwner()->GetTransformable().getTransform());
		}

		for (auto& it2 : it.second->m_textEntries)
		{
			m_renderWindow->draw(*it2.second.m_drawable, it2.second.m_owner->GetOwner()->GetTransformable().getTransform());
		}
	}
}

void RenderSystem::AddSprite(sf::Sprite* sprite, EntityComponent* owner, int layer)
{
	RenderEntry entry;
	entry.m_owner = owner;
	entry.m_drawable = sprite;
	m_ownerToLayer[owner] = layer;
	if (!m_renderLayers.contains(layer))
	{
		std::shared_ptr<RenderLayer> newLayer = std::make_shared<RenderLayer>();
		m_renderLayersVector.push_back(newLayer);
		m_renderLayers[layer] = newLayer.get();
	}
	m_renderLayers[layer]->m_spriteEntries[owner->m_id] = entry;

	DebugPrint("Added Sprite to EntityComponent (c_id " + std::to_string(owner->m_id) + ")", TextColor::Yellow, DebugChannel::RenderSystem, __FILE__, __LINE__);
}

void RenderSystem::AddText(sf::Text* text, EntityComponent* owner, int layer)
{
	RenderEntry entry;
	entry.m_owner = owner;
	entry.m_drawable = text;
	m_ownerToLayer[owner] = layer;
	if (!m_renderLayers.contains(layer))
	{
		std::shared_ptr<RenderLayer> newLayer = std::make_shared<RenderLayer>();
		m_renderLayersVector.push_back(newLayer);
		m_renderLayers[layer] = newLayer.get();
	}
	m_renderLayers[layer]->m_textEntries[owner->m_id] = entry;

	DebugPrint("Added Text to EntityComponent (c_id " + std::to_string(owner->m_id) + ")", TextColor::Yellow, DebugChannel::RenderSystem, __FILE__, __LINE__);
}

void RenderSystem::AddShape(sf::Shape* drawable, EntityComponent* owner, int layer)
{
	RenderEntry entry;
	entry.m_owner = owner;
	entry.m_drawable = drawable;
	m_ownerToLayer[owner] = layer;
	if (!m_renderLayers.contains(layer))
	{
		std::shared_ptr<RenderLayer> newLayer = std::make_shared<RenderLayer>();
		m_renderLayersVector.push_back(newLayer);
		m_renderLayers[layer] = newLayer.get();
	}
	m_renderLayers[layer]->m_shapeEntries[owner->m_id] = entry;

	DebugPrint("Added Shape to EntityComponent (c_id " + std::to_string(owner->m_id) + ")", TextColor::Yellow, DebugChannel::RenderSystem, __FILE__, __LINE__);
}

void RenderSystem::AddShader(sf::Drawable* drawable, sf::Shader* shader, EntityComponent* owner, int layer)
{
	RenderEntry entry;
	entry.m_owner = owner;
	entry.m_shader = shader;
	entry.m_drawable = drawable;
	m_ownerToLayer[owner] = layer;
	if (!m_renderLayers.contains(layer))
	{
		std::shared_ptr<RenderLayer> newLayer = std::make_shared<RenderLayer>();
		m_renderLayersVector.push_back(newLayer);
		m_renderLayers[layer] = newLayer.get();
	}
	m_renderLayers[layer]->m_shaderEntries[owner->m_id] = entry;

	DebugPrint("Added Shader to EntityComponent (c_id " + std::to_string(owner->m_id) + ")", TextColor::Yellow, DebugChannel::RenderSystem, __FILE__, __LINE__);
}

//void RenderSystem::RemoveEntry(EntityComponent* owner, EntryType type)
//{
//	RenderLayer& layer = m_renderLayers[m_ownerToLayer[owner]];
//
//	switch (type)
//	{
//
//	}
//	std::map<int, RenderEntry>& entries = layer.m_spriteEntries;
//
//	if (auto search = entries.find(owner->m_id); search != entries.end())
//	{
//		entries.erase(search);
//		DebugPrint("Removed Sprite from EntityComponent (c_id " + std::to_string(owner->m_id) + ")", TextColor::Yellow, DebugChannel::RenderSystem, __FILE__, __LINE__);
//	}
//	else
//	{
//		DebugPrint("Couldn't remove Sprite from EntityComponent (c_id " + std::to_string(owner->m_id) + ")", TextColor::Red, DebugChannel::RenderSystem, __FILE__, __LINE__, 1);
//	}
//}

void RenderSystem::RemoveSprite(EntityComponent* owner)
{
	RenderLayer* layer = m_renderLayers[m_ownerToLayer[owner]];
	std::map<int, RenderEntry>& entries = layer->m_spriteEntries;

	if (auto search = entries.find(owner->m_id); search != entries.end())
	{
		entries.erase(search);
		DebugPrint("Removed Sprite from EntityComponent (c_id " + std::to_string(owner->m_id) + ")", TextColor::Yellow, DebugChannel::RenderSystem, __FILE__, __LINE__);
	}
	else
	{
		DebugPrint("Couldn't remove Sprite from EntityComponent (c_id " + std::to_string(owner->m_id) + ")", TextColor::Red, DebugChannel::RenderSystem, __FILE__, __LINE__, 1);
	}
}

void RenderSystem::RemoveText(EntityComponent* owner)
{
	RenderLayer* layer = m_renderLayers[m_ownerToLayer[owner]];
	std::map<int, RenderEntry>& entries = layer->m_textEntries;

	if (auto search = entries.find(owner->m_id); search != entries.end())
	{
		entries.erase(search);
		DebugPrint("Removed Text from EntityComponent (c_id " + std::to_string(owner->m_id) + ")", TextColor::Yellow, DebugChannel::RenderSystem, __FILE__, __LINE__);
	}
	else
	{
		DebugPrint("Couldn't remove Text from EntityComponent (c_id " + std::to_string(owner->m_id) + ")", TextColor::Red, DebugChannel::RenderSystem, __FILE__, __LINE__, 1);
	}
}

void RenderSystem::RemoveShape(EntityComponent* owner)
{
	RenderLayer* layer = m_renderLayers[m_ownerToLayer[owner]];
	std::map<int, RenderEntry>& entries = layer->m_shapeEntries;

	if (auto search = entries.find(owner->m_id); search != entries.end())
	{
		entries.erase(search);
		DebugPrint("Removed Shape from EntityComponent (c_id " + std::to_string(owner->m_id) + ")", TextColor::Yellow, DebugChannel::RenderSystem, __FILE__, __LINE__);
	}
	else
	{
		DebugPrint("Couldn't remove Shape from EntityComponent (c_id " + std::to_string(owner->m_id) + ")", TextColor::Red, DebugChannel::RenderSystem, __FILE__, __LINE__, 1);
	}
}

void RenderSystem::RemoveShader(EntityComponent* owner)
{
	RenderLayer* layer = m_renderLayers[m_ownerToLayer[owner]];
	std::map<int, RenderEntry>& entries = layer->m_shaderEntries;

	if (auto search = entries.find(owner->m_id); search != entries.end())
	{
		entries.erase(search);
		DebugPrint("Removed Shader from EntityComponent (c_id " + std::to_string(owner->m_id) + ")", TextColor::Yellow, DebugChannel::RenderSystem, __FILE__, __LINE__);
	}
	else
	{
		DebugPrint("Couldn't remove Shader from EntityComponent (c_id " + std::to_string(owner->m_id) + ")", TextColor::Red, DebugChannel::RenderSystem, __FILE__, __LINE__, 1);
	}
}
