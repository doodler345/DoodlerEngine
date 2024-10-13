#include "World.h"

#include <string>


void World::EntityInit()
{
	m_shaderSpaceSize = Engine::GetInstance()->GetRenderWindow().getSize();

	std::string vertPath = "../Resources/Shaders/World_Image.vert";
	std::string fragPath = "../Resources/Shaders/World_Image.frag";

	// Setup ShaderSpace
	m_shaderSpaceLocal.setPrimitiveType(sf::PrimitiveType::Quads);
	m_shaderSpaceLocal.resize(4);
	float centerOffset[2] = { -0.5f * m_shaderSpaceSize.x , -0.5f * m_shaderSpaceSize.y }; // used to center image

	m_shaderSpaceLocal[0].position = sf::Vector2f(centerOffset[0], centerOffset[1]);
	m_shaderSpaceLocal[1].position = sf::Vector2f(m_shaderSpaceSize.x + centerOffset[0], centerOffset[1]);
	m_shaderSpaceLocal[2].position = sf::Vector2f(m_shaderSpaceSize.x + centerOffset[0], m_shaderSpaceSize.y + centerOffset[1]);
	m_shaderSpaceLocal[3].position = sf::Vector2f(0 + centerOffset[0], m_shaderSpaceSize.y + centerOffset[1]);

	m_shaderSpaceLocal[0].texCoords = sf::Vector2f(0, 0);
	m_shaderSpaceLocal[1].texCoords = sf::Vector2f(1, 0);
	m_shaderSpaceLocal[2].texCoords = sf::Vector2f(1, 1);
	m_shaderSpaceLocal[3].texCoords = sf::Vector2f(0, 1);

	std::shared_ptr<RectangleComponent> rectangleComponent = std::make_shared<RectangleComponent>(this);
	AddComponent(rectangleComponent);
	this->m_rectangleComponent = rectangleComponent.get();
	m_rectangleComponent->GetRectangle()->setFillColor(sf::Color(0, 0, 0, 0));
	m_rectangleComponent->GetRectangle()->setSize((sf::Vector2f)m_shaderSpaceSize);
	m_rectangleComponent->Center();

	UpdateBorders();

	// Add ShaderComponent
	std::shared_ptr<ShaderComponent> component = std::make_shared<ShaderComponent>(&m_shaderSpaceLocal, vertPath, fragPath, this);
	AddComponent<ShaderComponent>(component);

	m_shaderComponent = component.get();
}

void World::Scale(float factor)
{
	GetTransform().scale(factor, factor);
	m_rectangleComponent->GetRectangle()->setSize((sf::Vector2f)m_shaderSpaceSize * factor);
	m_rectangleComponent->Center();

	UpdateBorders();
}

void World::UpdateBorders()
{
	const float* imageMatrix = GetTransform().getMatrix();
	sf::FloatRect imageRect = GetComponent<RectangleComponent>()->GetRectangle()->getGlobalBounds();
	m_worldImageBorders[0] = imageMatrix[12] + imageRect.left;
	m_worldImageBorders[1] = imageMatrix[12] - imageRect.left;
	m_worldImageBorders[2] = imageMatrix[13] + imageRect.top;
	m_worldImageBorders[3] = imageMatrix[13] - imageRect.top;
}

void World::DestroyDerived()
{
}
