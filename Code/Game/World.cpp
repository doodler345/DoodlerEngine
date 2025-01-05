#include "World.h"

#include <string>


void World::EntityInit()
{
	m_shaderSpaceSize = Engine::GetInstance()->GetRenderWindow().getSize();

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

	// GroundTile BMP
	m_bmpGroundTile.Read(m_pathBmpGroundTile);
	m_groundTileSize = m_bmpGroundTile.GetSize();
	if (m_groundTileSize.x != m_groundTileSize.y)
	{
		DebugPrint("Ground Tile BMP Size needs to be squared", TextColor::Red, DebugChannel::Game, __FILE__, __LINE__, true);
		return;
	}
}

void World::Scale(float factor)
{
	GetTransform().scale(factor, factor);
	m_rectangleComponent->GetRectangle()->setSize((sf::Vector2f)m_shaderSpaceSize * factor);
	m_rectangleComponent->Center();

	UpdateBorders();
}

void World::Setup(unsigned int width, unsigned int height)
{
	m_worldWidth = width;
	m_worldHeight = height;
	m_numOfPixels = width * height;
	m_pixelValues.resize(m_numOfPixels);
	m_texture.create(width, height);

	m_pixelColors = std::make_unique<sf::Uint8>(m_numOfPixels * 4);
	for (int i = 0; i < m_numOfPixels; i++)
	{
		m_pixelColors.get()[i * 4 + 3] = sf::Uint8(255); // alpha
	}
}

void World::Setup(const char* bmpPath)
{
	BMPImage bmp;
	bmp.Read(bmpPath);

	m_worldWidth = bmp.GetSize().x;
	m_worldHeight = bmp.GetSize().y;
	m_numOfPixels = m_worldWidth * m_worldHeight;
	m_pixelValues.resize(m_numOfPixels);
	m_texture.create(m_worldWidth, m_worldHeight);

	m_pixelColors = std::make_unique<sf::Uint8>(m_numOfPixels * 4);

	int i = 0;
	for (int y = 0; y < m_worldHeight; y++)
	{
		for (int x = 0; x < m_worldWidth; x++)
		{
			bmp::Color color = bmp.GetColor(x, y);
			m_pixelColors.get()[i * 4 + 0] = color.r;
			m_pixelColors.get()[i * 4 + 1] = color.g;
			m_pixelColors.get()[i * 4 + 2] = color.b;
			m_pixelColors.get()[i * 4 + 3] = sf::Uint8(255); // alpha
			
			i++;
		}

	}
}

void World::UpdateTexture()
{
	m_texture.update(m_pixelColors.get());
	m_shaderComponent->m_shader.setUniform("tex", m_texture);
}

void World::Colorize(bool useColor)
{
	if (useColor)
	{
		bool everyBackgroundHeaven = true;
		bool everyBackgroundCave = false;

		/////////////////////////////////////////////
			// Visual Pass 1 (after terrain is fully generated)

		std::vector<int> caveFloorIndices;
		int i = 0;

		for (int y = 0; y < m_worldHeight; y++)
		{
			for (int x = 0; x < m_worldWidth; x++)
			{
				if (m_pixelValues[i] == 1) // wall
				{
					bool* neighbours = GetNeighbours(i);
					if (neighbours[1] && m_pixelValues[i - m_worldWidth] == 0) // floor in cave (stone)
					{
						caveFloorIndices.push_back(i);
					}
					else // cave
					{
						bmp::Color col = m_bmpGroundTile.GetColor(x % m_groundTileSize.x, y % m_groundTileSize.y);
						m_pixelColors.get()[i * 4 + 0] = sf::Uint8(col.r * 255);
						m_pixelColors.get()[i * 4 + 1] = sf::Uint8(col.g * 255);
						m_pixelColors.get()[i * 4 + 2] = sf::Uint8(col.b * 255);
					}

				}
				else // empty space
				{
					float fade = 1.0f - (float)y / m_worldHeight * 0.3f;
					if (everyBackgroundHeaven)
					{
						m_pixelColors.get()[i * 4 + 0] = sf::Uint8(40 * fade);
						m_pixelColors.get()[i * 4 + 1] = sf::Uint8(40 * fade);
						m_pixelColors.get()[i * 4 + 2] = sf::Uint8(230 * fade);
					}
					else if (everyBackgroundCave)
					{
						m_pixelColors.get()[i * 4 + 0] = sf::Uint8(35);
						m_pixelColors.get()[i * 4 + 1] = sf::Uint8(20);
						m_pixelColors.get()[i * 4 + 2] = sf::Uint8(5);
					}
					//else
					//{
					//	if (y < m_groundHeights[x]) //heaven
					//	{
					//		m_pixelColors[i * 4 + 0] = sf::Uint8(40 * fade);
					//		m_pixelColors[i * 4 + 1] = sf::Uint8(40 * fade);
					//		m_pixelColors[i * 4 + 2] = sf::Uint8(230 * fade);
					//	}
					//	else // cave background
					//	{
					//		m_pixelColors[i * 4 + 0] = sf::Uint8(35);
					//		m_pixelColors[i * 4 + 1] = sf::Uint8(20);
					//		m_pixelColors[i * 4 + 2] = sf::Uint8(5);
					//	}
					//}
				}


				i++;
			}
		}
	


		//cave floor
		for (int i = 0; i < caveFloorIndices.size(); i++)
		{
			int level = 0;
			int y = caveFloorIndices[i];

			do
			{
				m_pixelColors.get()[y * 4 + 0] = sf::Uint8(200);
				m_pixelColors.get()[y * 4 + 1] = sf::Uint8(200);
				m_pixelColors.get()[y * 4 + 2] = sf::Uint8(200);

				y += m_worldWidth;
				level++;

				if (y >= m_numOfPixels - 1) // window ground reached
				{
					break;
				}
			} while (GetNeighbours(y)[7] && level < m_caveStoneDepth);
		}

		/////////////////////////////////////////////
		// Visual Pass 2 

		for (int x = 0; x < m_worldWidth; x++)
		{
			bool groundReached = false;
			//int y = m_groundHeights[x]; // upper world edge as starting point
			int y = 0;
			int i = y * m_worldWidth + x;

			while (m_pixelValues[i] == 0)
			{
				i += m_worldWidth;

				if (i >= m_numOfPixels - 1) // window ground reached
				{
					groundReached = true;
					break;
				}
			}


			if (!groundReached) // grass at bottom
			{
				int level = 0;
				while (m_pixelValues[i] == 1 && level < m_grasDepth)
				{
					m_pixelColors.get()[i * 4 + 0] = sf::Uint8(0);
					m_pixelColors.get()[i * 4 + 1] = sf::Uint8(255);
					m_pixelColors.get()[i * 4 + 2] = sf::Uint8(0);

					i += m_worldWidth;
					level++;

					if (i >= m_numOfPixels - 1) // window ground reached
					{
						break;
					}
				}
			}
		}
	}

	else
	{
		sf::Uint8 color;
		int i = 0;

		for (int y = 0; y < m_worldHeight; y++)
		{
			for (int x = 0; x < m_worldWidth; x++)
			{
				if (m_pixelValues[i] == 0)
					color = 0;
				else
					color = 255;

				m_pixelColors.get()[i * 4 + 0] = color;
				m_pixelColors.get()[i * 4 + 1] = color;
				m_pixelColors.get()[i * 4 + 2] = color;

				i++;
			}
		}
	}

	UpdateTexture();
}

bool* World::GetNeighbours(int index, bool onlyLeftandRight)
{
	short edge[] = { 0,0 };

	edge[0] = 0;
	edge[1] = 0;

	// Check for Edges
	if (index % m_worldWidth == 0) // left edge
	{
		edge[0] = -1;
	}
	else if ((index + 1) % m_worldWidth == 0) // right edge
	{
		edge[0] = 1;
	}

	if (!onlyLeftandRight)
	{
		if (index < m_worldWidth) // upper edge
		{
			edge[1] = -1;
		}
		else if (index >= (m_worldWidth * m_worldHeight - m_worldWidth)) // lower edge
		{
			edge[1] = 1;
		}
	}

	// Set Neighbours
	for (int index = 0; index < 9; index++)
	{
		m_neighbours[index] = true;
	}
	m_neighbours[4] = false; //middle one 

	//clockwise, starting at top-left neighbour
	//	0 1 2
	//	3 4 5
	//	6 7 8

	if (edge[0] == -1)
	{
		// Left Edge
		m_neighbours[0] = false;
		m_neighbours[3] = false;
		m_neighbours[6] = false;
	}
	else if (edge[0] == 1)
	{
		m_neighbours[2] = false;
		m_neighbours[5] = false;
		m_neighbours[8] = false;
	}

	if (!onlyLeftandRight)
	{
		if (edge[1] == -1)
		{
			// Upper Edge
			m_neighbours[0] = false;
			m_neighbours[1] = false;
			m_neighbours[2] = false;
		}
		else if (edge[1] == 1)
		{
			// Lower Edge
			m_neighbours[6] = false;
			m_neighbours[7] = false;
			m_neighbours[8] = false;
		}
	}

	return m_neighbours;
}

bool* World::GetNeighbours2(int index, bool onlyLeftandRight)
{
	short edge[] = { 0,0 };

	edge[0] = 0;
	edge[1] = 0;

	// Check for Edges
	if (index % m_worldWidth == 0) // left edge
	{
		edge[0] = -1;
	}
	else if ((index + 1) % m_worldWidth == 0) // right edge
	{
		edge[0] = 1;
	}

	if (!onlyLeftandRight)
	{
		if (index < m_worldWidth) // upper edge
		{
			edge[1] = -1;
		}
		else if (index >= (m_worldWidth * m_worldHeight - m_worldWidth)) // lower edge
		{
			edge[1] = 1;
		}
	}

	// Set Neighbours
	for (int i = 0; i < 9; i++)
	{
		m_neighbours2[i] = true;
	}
	m_neighbours2[4] = false; //middle one 

	//clockwise, starting at top-left neighbour
	//	0 1 2
	//	3 4 5
	//	6 7 8

	if (edge[0] == -1)
	{
		// Left Edge
		m_neighbours2[0] = false;
		m_neighbours2[3] = false;
		m_neighbours2[6] = false;
	}
	else if (edge[0] == 1)
	{
		m_neighbours2[2] = false;
		m_neighbours2[5] = false;
		m_neighbours2[8] = false;
	}

	if (!onlyLeftandRight)
	{
		if (edge[1] == -1)
		{
			// Upper Edge
			m_neighbours2[0] = false;
			m_neighbours2[1] = false;
			m_neighbours2[2] = false;
		}
		else if (edge[1] == 1)
		{
			// Lower Edge
			m_neighbours2[6] = false;
			m_neighbours2[7] = false;
			m_neighbours2[8] = false;
		}
	}

	return m_neighbours2;
}

bool* World::GetNeighbours3(int index, bool onlyLeftandRight)
{
	short edge[] = { 0,0 };

	edge[0] = 0;
	edge[1] = 0;

	// Check for Edges
	if (index % m_worldWidth == 0) // left edge
	{
		edge[0] = -1;
	}
	else if ((index + 1) % m_worldWidth == 0) // right edge
	{
		edge[0] = 1;
	}

	if (!onlyLeftandRight)
	{
		if (index < m_worldWidth) // upper edge
		{
			edge[1] = -1;
		}
		else if (index >= (m_worldWidth * m_worldHeight - m_worldWidth)) // lower edge
		{
			edge[1] = 1;
		}
	}

	// Set Neighbours
	for (int i = 0; i < 9; i++)
	{
		m_neighbours3[i] = true;
	}
	m_neighbours3[4] = false; //middle one 

	//clockwise, starting at top-left neighbour
	//	0 1 2
	//	3 4 5
	//	6 7 8

	if (edge[0] == -1)
	{
		// Left Edge
		m_neighbours3[0] = false;
		m_neighbours3[3] = false;
		m_neighbours3[6] = false;
	}
	else if (edge[0] == 1)
	{
		m_neighbours3[2] = false;
		m_neighbours3[5] = false;
		m_neighbours3[8] = false;
	}

	if (!onlyLeftandRight)
	{
		if (edge[1] == -1)
		{
			// Upper Edge
			m_neighbours3[0] = false;
			m_neighbours3[1] = false;
			m_neighbours3[2] = false;
		}
		else if (edge[1] == 1)
		{
			// Lower Edge
			m_neighbours3[6] = false;
			m_neighbours3[7] = false;
			m_neighbours3[8] = false;
		}
	}

	return m_neighbours3;
}

bool* World::GetNeighbours4(int index, bool onlyLeftandRight)
{
	short edge[] = { 0,0 };

	edge[0] = 0;
	edge[1] = 0;

	// Check for Edges
	if (index % m_worldWidth == 0) // left edge
	{
		edge[0] = -1;
	}
	else if ((index + 1) % m_worldWidth == 0) // right edge
	{
		edge[0] = 1;
	}

	if (!onlyLeftandRight)
	{
		if (index < m_worldWidth) // upper edge
		{
			edge[1] = -1;
		}
		else if (index >= (m_worldWidth * m_worldHeight - m_worldWidth)) // lower edge
		{
			edge[1] = 1;
		}
	}

	// Set Neighbours
	for (int i = 0; i < 9; i++)
	{
		m_neighbours4[i] = true;
	}
	m_neighbours4[4] = false; //middle one 

	//clockwise, starting at top-left neighbour
	//	0 1 2
	//	3 4 5
	//	6 7 8

	if (edge[0] == -1)
	{
		// Left Edge
		m_neighbours4[0] = false;
		m_neighbours4[3] = false;
		m_neighbours4[6] = false;
	}
	else if (edge[0] == 1)
	{
		m_neighbours4[2] = false;
		m_neighbours4[5] = false;
		m_neighbours4[8] = false;
	}

	if (!onlyLeftandRight)
	{
		if (edge[1] == -1)
		{
			// Upper Edge
			m_neighbours4[0] = false;
			m_neighbours4[1] = false;
			m_neighbours4[2] = false;
		}
		else if (edge[1] == 1)
		{
			// Lower Edge
			m_neighbours4[6] = false;
			m_neighbours4[7] = false;
			m_neighbours4[8] = false;
		}
	}

	return m_neighbours4;
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
