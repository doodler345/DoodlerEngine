#include "ShaderComponent.h"

#include "../../Debug.h"
#include "../../Engine.h"
#include "../../RenderSystem.h"

ShaderComponent::ShaderComponent(sf::Drawable* drawable, std::string& vertShaderPath, std::string& fragShaderPath, Entity* owner, int renderLayer)
{
	std::string type = typeid(this).name();
	m_name = type;

	m_ownerEntity = owner;

	m_drawable = drawable;
	m_vertPath = vertShaderPath;
	m_fragPath = fragShaderPath;
	m_renderLayer = renderLayer;

	m_renderSystem = &Engine::GetInstance()->GetRenderSystem();

	if (!sf::Shader::isAvailable)
	{
		DebugPrint("Your Graphics-Card is too old, cannot run the shadercode", TextColor::Red, DebugChannel::Game, __FILE__, __LINE__, 1);
		return;
	}

	if (!m_shader.loadFromFile(m_vertPath, m_fragPath))
	{
		DebugPrint("Cannot load shaders", TextColor::Red, DebugChannel::Game, __FILE__, __LINE__, 1);
		return;
	}

	Engine::GetInstance()->GetRenderSystem().AddEntry(m_drawable, this, EntryType::ShaderEntry, m_renderLayer, &m_shader);
}

void ShaderComponent::SetVisibility(bool value)
{
	if (m_isVisible == value)
	{
		return;
	}

	m_isVisible = value;

	if (value)
	{
		m_renderSystem->AddEntry(m_drawable, this, EntryType::ShaderEntry, m_renderLayer, &m_shader);
	}
	else
	{
		m_renderSystem->RemoveEntry(this, EntryType::ShaderEntry);
	}
}

void ShaderComponent::SetRenderLayer(int layer)
{
	if (m_renderLayer == layer)
	{
		return;
	}

	m_renderLayer = layer;
		
	if (m_isVisible)
	{
		m_renderSystem->RemoveEntry(this, EntryType::ShaderEntry);
		m_renderSystem->AddEntry(m_drawable, this, EntryType::ShaderEntry, m_renderLayer, &m_shader);
	}
}

void ShaderComponent::ShutDown()
{
	if (m_isVisible)
	{
		Engine::GetInstance()->GetRenderSystem().RemoveEntry(this, EntryType::ShaderEntry);
	}
}
