#pragma once

#include <memory>
#include <SFML/Graphics.hpp>

#include "../Engine/Engine.h"
#include "../Engine/Entity/Entity.h"
#include "../Engine/EntityComponents/ShaderComponent.h"
#include "../Engine/EntityComponents/Primitives/RectangleComponent.h"

class World : public Entity
{
public:
	void EntityInit() override;
	void Scale(float factor);
	int m_worldImageBorders[4];
private:
	void UpdateBorders();
	void DestroyDerived() override;

	sf::VertexArray m_shaderSpaceLocal;
	RectangleComponent* m_rectangleComponent = nullptr;
	ShaderComponent* m_shaderComponent = nullptr;
	sf::Vector2u m_shaderSpaceSize;
};

