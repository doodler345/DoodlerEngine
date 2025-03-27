#include "ShaderComponent.h"

#include "../Debug.h"
#include "../Engine.h"
#include "../RenderSystem.h"

ShaderComponent::ShaderComponent(sf::Drawable* drawable, std::string& vertShaderPath, std::string& fragShaderPath, Entity* owner, int renderLayer)
{
	std::string type = typeid(this).name();
	m_name = type;

	m_ownerEntity = owner;

	m_vertPath = vertShaderPath;
	m_fragPath = fragShaderPath;

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

	Engine::GetInstance()->GetRenderSystem().AddShader(drawable, &m_shader, this, renderLayer);
}

void ShaderComponent::ShutDown()
{
	Engine::GetInstance()->GetRenderSystem().RemoveShader(this);
}
