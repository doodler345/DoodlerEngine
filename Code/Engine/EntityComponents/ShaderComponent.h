#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "../Entity/Entity.h"

class ShaderComponent : public EntityComponent
{
public:
	ShaderComponent(sf::Drawable* drawable, std::string& vertShaderPath, std::string& fragShaderPath, Entity* owner);
	
	void ShutDown() override;

	sf::Shader m_shader;
	sf::Drawable* m_drawable = nullptr;
private:
	std::string m_vertPath;
	std::string m_fragPath;
};

