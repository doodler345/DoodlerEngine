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

void RenderSystem::AddEntry(sf::Drawable* drawable, EntityComponent* owner, EntryType type, int layer, sf::Shader* shader)
{
	RenderEntry entry;
	entry.m_owner = owner;
	entry.m_drawable = drawable;
	entry.m_shader = shader;

	m_ownerToLayer[owner] = layer;
	if (!m_renderLayers.contains(layer))
	{
		std::shared_ptr<RenderLayer> newLayer = std::make_shared<RenderLayer>();
		m_renderLayersVector.push_back(newLayer);
		m_renderLayers[layer] = newLayer.get();
	}

	std::string entryType = "";
	switch (type)
	{
	case SpriteEntry:
		m_renderLayers[layer]->m_spriteEntries[owner->m_id] = entry;
		entryType = "Sprite";
		break;
	case TextEntry:
		m_renderLayers[layer]->m_textEntries[owner->m_id] = entry;
		entryType = "Text";
		break;
	case ShapeEntry:
		m_renderLayers[layer]->m_shapeEntries[owner->m_id] = entry;
		entryType = "Shape";
		break;
	case ShaderEntry:
		m_renderLayers[layer]->m_shaderEntries[owner->m_id] = entry;
		entryType = "Shader";
		break;
	}

	DebugPrint("Added " + entryType + " to EntityComponent(c_id " + std::to_string(owner->m_id) + ")", TextColor::Yellow, DebugChannel::RenderSystem, __FILE__, __LINE__);
}

void RenderSystem::RemoveEntry(EntityComponent* owner, EntryType type)
{
	RenderLayer* layer = m_renderLayers[m_ownerToLayer[owner]];
	std::map<int, RenderEntry>* entries = nullptr;
	std::string entryType = "";

	switch (type)
	{
	case SpriteEntry:
		entries = &layer->m_spriteEntries;
		entryType = "Sprite";
		break;
	case TextEntry:
		entries = &layer->m_textEntries;
		entryType = "Text";
		break;
	case ShapeEntry:
		entries = &layer->m_shapeEntries;
		entryType = "Shape";
		break;
	case ShaderEntry:
		entries = &layer->m_shaderEntries;
		entryType = "Shader";
		break;
	}

	if (auto search = entries->find(owner->m_id); search != entries->end())
	{
		entries->erase(search);
		DebugPrint("Removed " + entryType + " from EntityComponent(c_id " + std::to_string(owner->m_id) + ")", TextColor::Yellow, DebugChannel::RenderSystem, __FILE__, __LINE__);
	}
	else
	{
		DebugPrint("Couldn't remove Sprite from EntityComponent (c_id " + std::to_string(owner->m_id) + ")", TextColor::Red, DebugChannel::RenderSystem, __FILE__, __LINE__, 1);
	}
}

