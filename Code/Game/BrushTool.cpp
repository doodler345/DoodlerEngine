#include "BrushTool.h"
#include <numbers>
#include <memory>
#include <functional>

void BrushTool::EntityInit()
{
	std::shared_ptr<CircleComponent> circleComponent = std::make_shared<CircleComponent>(this);
	AddComponent(circleComponent);
	m_circleComponent = circleComponent.get();

	m_circleComponent->SetRadius(m_radius * 0.7f); //0.7f to match downscale of worldImage (yes this is a bad hack)
	sf::CircleShape* circle = m_circleComponent->GetCircle();
	circle->setFillColor(sf::Color(0, 0, 0, 0));
	circle->setOutlineColor(sf::Color(0, 0, 0, 0));
	circle->setOutlineThickness(m_outlineThickness);

	Engine::GetInstance()->GetInputManager().RegisterMouseScrollEntry(&GetTransform(), std::bind(&BrushTool::ChangeRadius, this, std::placeholders::_1));
}
void BrushTool::Setup(World* worldGenerationImage, Text* radiusInfo)
{
	m_worldGenerationImage = worldGenerationImage;
	m_pixelColors = worldGenerationImage->m_pixelColors.get();

	m_windowWidth = worldGenerationImage->m_worldWidth;
	m_worldHeight = worldGenerationImage->m_worldHeight;

	m_radiusInfo = radiusInfo;
}

void BrushTool::Activate(bool isActive)
{
	m_isActive = isActive;
	if (isActive) 
	{
		const float* matrix = GetTransform().getMatrix();
		m_prevMousePos = sf::Vector2i(matrix[12], matrix[13]);
		m_circleComponent->GetCircle()->setOutlineColor(m_circleColor);
	}
	else
	{
		m_circleComponent->GetCircle()->setOutlineColor(sf::Color(0,0,0,0));
	}
	
}

void BrushTool::Draw(bool isDrawing)
{
	m_isDrawing = isDrawing;
}

void BrushTool::Erase(bool eraseMode)
{
	m_eraseMode = eraseMode;
}

void BrushTool::ChangeRadius(float add)
{
	if (!m_isActive) return;

	float boostedAdd = add * m_scrollBoost;
	if (m_radius + boostedAdd <= m_minBrushRadius || m_radius + boostedAdd > m_maxBrushRadius) return;

	m_radius += boostedAdd;
	m_circleComponent->SetRadius((int)m_radius * 0.7f);

	m_radiusInfo->m_textComponent->SetText("Radius: " + std::to_string((int)m_radius));
}

void BrushTool::Update(float deltaTime)
{
	if (m_isActive)
	{
		m_mousePos = Engine::GetInstance()->GetInputManager().GetMousePos();
		m_deltaMousePos = m_mousePos - m_prevMousePos;
		m_prevMousePos = m_mousePos;

		GetTransform().translate((sf::Vector2f)m_deltaMousePos); //brushTool is owner of circle --> moves circle

		if (m_isDrawing || m_eraseMode)
		{
			sf::Vector2i imageSize;
			int* imageBorders = m_worldGenerationImage->m_worldImageBorders;
			imageSize.x = imageBorders[1] - imageBorders[0];
			imageSize.y = imageBorders[3] - imageBorders[2];

			sf::Vector2f pixelPosRelative;
			pixelPosRelative.x = (m_mousePos.x - imageBorders[0]) / (float)imageSize.x; 
			pixelPosRelative.y = (m_mousePos.y - imageBorders[2]) / (float)imageSize.y; 
			if (pixelPosRelative.x <= 0 || pixelPosRelative.x >= 1 || pixelPosRelative.y <= 0 || pixelPosRelative.y >= 1) 
			{
				return;
			}

			sf::Uint8 color = m_eraseMode ? sf::Uint8(0) : sf::Uint8(255);

			sf::Vector2i pixelCoordinates;
			pixelCoordinates.x = m_windowWidth * pixelPosRelative.x;
			pixelCoordinates.y = m_worldHeight * pixelPosRelative.y;

			int x;
			int prevX = pixelCoordinates.x + cos(0) * m_radius * -1;
			int deltaX;
			for (int i = -m_radius; i < m_radius; i++)
			{
				int xOffset		= cos((i + m_radius) / (2.0f * m_radius) * std::numbers::pi) * m_radius * -1;
				int yMaxOffset	= sin((i + m_radius) / (2.0f * m_radius) * std::numbers::pi) * m_radius;

				x = pixelCoordinates.x + xOffset;
				deltaX = x - prevX;

				// Sometimes deltaX is 0 or 2, because of the Cosine. That can lead to unnecessary calculations, or worse, horizontal gaps 
				// With substeps this cant happen anymore
				for (int xSubsteps = 1; xSubsteps < deltaX + 1; xSubsteps++) 
				{
					for (int yOffset = -yMaxOffset; yOffset < yMaxOffset; yOffset++)
					{
						int y = pixelCoordinates.y + yOffset;
						if (x >= m_windowWidth || x < 0 || y >= m_worldHeight || y < 0)
						{
							continue;
						}

						int pixelIndex = y * m_windowWidth + prevX + xSubsteps;


						m_worldGenerationImage->m_pixelValues[pixelIndex] = !m_eraseMode;
						m_pixelColors[pixelIndex * 4 + 0] = color;
						m_pixelColors[pixelIndex * 4 + 1] = color;
						m_pixelColors[pixelIndex * 4 + 2] = color;
						m_pixelColors[pixelIndex * 4 + 3] = sf::Uint8(255);
					}
				}

				prevX = x;
			}
		
			m_worldGenerationImage->UpdateTexture();
		}

	}

}

void BrushTool::DestroyDerived()
{
	Engine::GetInstance()->GetInputManager().UnregisterMouseScrollEntry(&GetTransform());
}