#include "WorldGenerator.h"

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <array>
#include <functional>
#include <thread>
#include <mutex>


#include "../../Engine/Engine.h"
#include "../../Engine/InputManager.h"
#include "../../Engine/UI_Elements/ButtonMenu.h"
#include "../../Engine/UI_Elements/Button.h"

#include "../GameManager.h"

float prevTime = 0;
float afterTime = 0;

void WorldGenerator::Init()
{
	///////////////////////////////////////////
	// WorldGenerator

	std::srand(time(NULL));

	m_name = "WorldGenerator";

	InputManager& inputManager = Engine::GetInstance()->GetInputManager();
	sf::RenderWindow& window = Engine::GetInstance()->GetRenderWindow();
	m_windowWidth	= window.getSize().x;
	m_windowHeight	= window.getSize().y;
	m_numOfPixels = m_windowWidth * m_windowHeight;
	m_groundHeights.resize(m_windowWidth);



	///////////////////////////////////////////
	// BMPs

	m_bmpWorld.Read(m_pathBmpWorld); //just to resize Image
	m_bmpGroundTile.Read(m_pathBmpGroundTile);

	m_tileSize = m_bmpGroundTile.GetSize();
	if (m_tileSize.x != m_tileSize.y)
	{
		DebugPrint("Ground Tile BMP Size needs to be squared", TextColor::Red, DebugChannel::Game, __FILE__, __LINE__, true);
		return;
	}



	///////////////////////////////////////////
	// Entities SETUP

	// Instantiations
	m_worldGenerationImage =		Instantiate(World, WorldImage);
	m_inputField =					Instantiate(InputField, BMPNameInputField);
	m_buttonMenu =					Instantiate(ButtonMenu, GenerateWorldButtonMenu);
	m_densityInfo =					Instantiate(Text, DensityTxt);
	m_itterationInfo =				Instantiate(Text, IterationsTxt);
	m_itterationStepsInfo =			Instantiate(Text, IterationStepsTxt);
	m_brushRadiusInfo =				Instantiate(Text, BrushRadiusTxt);
	m_brushTool =					Instantiate(BrushTool, brush);
	m_sliderIterationSteps =		Instantiate(Slider, sliderIterationSteps);
	m_sliderDensity =				Instantiate(Slider, sliderDensity);
	m_sliderIterationStepsHandler =	Instantiate(Empty, sliderIterationStepsHandler);
	m_sliderDensityHandler =		Instantiate(Empty, sliderDensityHandler);


	// World Image Setup	
	m_pixelColors = new sf::Uint8[m_numOfPixels * 4];
	m_pixelEmpty.resize(m_numOfPixels);
	m_isLit.resize(m_numOfPixels);
	m_texture.create(m_windowWidth, m_windowHeight);
	
	for (int i = 0; i < m_numOfPixels; i++)
	{
		m_pixelColors[i * 4 + 3] = sf::Uint8(255); // alpha
	}

	inputManager.RegisterRectangleEntry(
		sf::Mouse::Button::Left, &m_worldGenerationImage->GetTransform(), m_worldGenerationImage->GetComponent<RectangleComponent>()->GetRectangle(), 
		std::bind(&WorldGenerator::Draw, this, std::placeholders::_1, std::placeholders::_2));
	inputManager.RegisterRectangleEntry(
		sf::Mouse::Button::Right, &m_worldGenerationImage->GetTransform(), m_worldGenerationImage->GetComponent<RectangleComponent>()->GetRectangle(),
		std::bind(&WorldGenerator::Erase, this, std::placeholders::_1, std::placeholders::_2));


	// BrushTool Setup
	m_brushTool->Setup(m_worldGenerationImage, m_pixelColors, &m_pixelEmpty[0], &m_texture, m_windowHeight, m_windowWidth, m_brushRadiusInfo);


	// Slider Setup
	m_sliderDensity->Setup(m_sliderDensityHandler, sf::Vector2f(100, 20), std::bind(&WorldGenerator::ChangeDensity, this, std::placeholders::_1));
	m_sliderDensity->SetValue(0.65f);

	m_sliderIterationSteps->Setup(m_sliderIterationStepsHandler, sf::Vector2f(100,20), std::bind(&WorldGenerator::ChangeIterationSteps, this, std::placeholders::_1));
	m_sliderIterationSteps->SetValue(0.35f);


	// InputField Setup
	std::ifstream inputStream(m_pathBmpIndex);
	std::string bmpIndex;
	std::getline(inputStream, bmpIndex);

	m_inputField->SetText("GeneratedMap" + bmpIndex);
	m_inputField->SetFileEnding(".bmp");
	m_inputField->SetMaxChars(15);


	// Button Setup
	std::array<KEY, 3> navKeys =
	{
		KEY::Left,
		KEY::Right,
		KEY::Enter
	};

	std::vector<Button*> buttons = m_buttonMenu->InitMenu(6, navKeys);
	int indexer = 0;
	buttons[indexer++]->SetText("Generate");
	buttons[indexer++]->SetText("Iterate");
	buttons[indexer++]->SetSprite("../Resources/UI/Brush.png");
	buttons[indexer++]->SetSprite("../Resources/UI/Undo.png");
	buttons[indexer++]->SetSprite("../Resources/UI/Colorize.png");
	buttons[indexer++]->SetText("Save");

	indexer = 0;
	buttons[indexer++]->SetButtonCallback(std::bind(&WorldGenerator::GenerateNoisemap, this));
	buttons[indexer++]->SetButtonCallback(std::bind(&WorldGenerator::CellularAutomataIteration, this));
	buttons[indexer++]->SetButtonCallback(std::bind(&WorldGenerator::ToggleBrushMode, this));
	buttons[indexer++]->SetButtonCallback(std::bind(&WorldGenerator::Undo, this));
	buttons[indexer++]->SetButtonCallback(std::bind(&WorldGenerator::ColorizeWorld, this, 1));
	buttons[indexer++]->SetButtonCallback(std::bind(&WorldGenerator::SaveWorld, this));


	// Text Setup
	m_densityInfo->m_textComponent->SetText			("Density: " + std::to_string(m_density));
	m_itterationInfo->m_textComponent->SetText		("Iterations: " + std::to_string(m_itterations));
	m_itterationStepsInfo->m_textComponent->SetText	("Steps: " + std::to_string(m_itterationSteps));
	m_brushRadiusInfo->m_textComponent->SetText		("Radius: " + std::to_string(m_brushTool->GetRadius()));
	
	m_densityInfo->m_textComponent->SetTextColor			(sf::Color::Black);
	m_itterationInfo->m_textComponent->SetTextColor		(sf::Color::Black);
	m_itterationStepsInfo->m_textComponent->SetTextColor	(sf::Color::Black);
	m_brushRadiusInfo->m_textComponent->SetTextColor		(sf::Color::Black);
	



	///////////////////////////////////////////
	// Entities LAYOUT

	float xUISettings = m_windowWidth * 0.13f;
	float yDensity = m_windowHeight * 0.2f;
	float yGenerate = m_windowHeight * 0.25f;
	float yIterationSteps = m_windowHeight * 0.35f;
	float yIterate = m_windowHeight * 0.4f;
	float yBrush = m_windowHeight * 0.6f;
	float yUndo = m_windowHeight * 0.8f;
	float ySave = m_windowHeight * 0.95f;
	float yOffset = m_windowHeight * 0.05f;

	m_worldGenerationImage->GetTransform().translate(window.getSize().x * 0.6f, window.getSize().y * 0.5f);
	m_worldGenerationImage->Scale(0.7f);

	m_inputField->GetTransform().translate(m_windowWidth * 0.5f, ySave);

	m_sliderDensity->Translate(xUISettings + 70, yDensity);
	m_sliderIterationSteps->Translate(xUISettings + 70, yIterationSteps);

	indexer = 0;
	buttons[indexer++]->GetTransform().translate(xUISettings, yGenerate);
	buttons[indexer++]->GetTransform().translate(xUISettings, yIterate + yOffset);
	buttons[indexer++]->GetTransform().translate(xUISettings, yBrush);
	buttons[indexer++]->GetTransform().translate(xUISettings - 50, yUndo);
	buttons[indexer++]->GetTransform().translate(xUISettings + 50, yUndo);
	buttons[indexer++]->GetTransform().translate(m_windowWidth * 0.5f + 150, ySave);

	m_densityInfo->GetTransform().translate(xUISettings - 50, yDensity);
	m_itterationInfo->GetTransform().translate(xUISettings, yIterate);
	m_itterationStepsInfo->GetTransform().translate(xUISettings - 50, yIterationSteps);
	m_brushRadiusInfo->GetTransform().translate(xUISettings, yBrush + yOffset * 1.3f);



	///////////////////////////////////////////
	//Finalizing
	GenerateNoisemap();
	m_initializing = false;

}

void WorldGenerator::GenerateNoisemap()
{
	PushOldValues();

			prevTime = Engine::GetInstance()->GetTimeAsSeconds();

	// Generate Noise-Map

	int texelColor;
	int rnd;
	int i = 0;

	// set floor (y) along the x-axis
	for (int i = 0; i < m_windowWidth; i++) // set base floor
	{
		m_groundHeights[i] = m_windowHeight * m_upperWorldPercentBase / 100;
	}

	int numOfSins = 3;
	std::vector<int> changesInHeight(m_windowWidth, 0);
	for (int i = 0; i < numOfSins; i++) // change Heigths
	{
		float rndSin = rand() % 200 + 0.0001f;
		float rndAmp = rand() % 50;

		for (int x = 0; x < m_windowWidth; x++) 
		{
			//in sin(x / y) * z: lower y -> higher freq, z = amp
			changesInHeight[x] += sin(float(x) / (rndSin)) * rndAmp;
			m_groundHeights[x] += changesInHeight[x];
		}
	}

	// generate Noise
	for (int y = 0; y < m_windowHeight; y++)
	{
		for (int x = 0; x < m_windowWidth; x++)
		{
			if (y < m_groundHeights[x]) // upper World
			{
				m_pixelEmpty[i] = 1;
			}
			else // caves
			{
				rnd = rand() % 100;
				texelColor = rnd >= m_density ? 1 : 0;

				m_pixelEmpty[i] = texelColor;
			}
			
			i++;
		}
	}
	
	m_itterations = 0;
	m_itterationInfo->m_textComponent->SetText("Iterations: " + std::to_string(m_itterations));

			afterTime = Engine::GetInstance()->GetTimeAsSeconds();
			std::cout << "Generate Noise Map: " << afterTime - prevTime << std::endl;
	
	ColorizeWorld(false);
}

void WorldGenerator::CellularAutomataIteration()
{
	PushOldValues();

	std::vector<int> pixelEmpty_copy = m_pixelEmpty;

	// iterate
	for (int r = 0; r < m_itterationSteps; r++)
	{
		prevTime = Engine::GetInstance()->GetTimeAsSeconds();
		
		int quaterRow = m_windowWidth / 4.f;
		int quaterCol = m_windowHeight / 4.f;

		
		bool useFourThreads = 0;

		if (useFourThreads)
		{
			std::thread t1(&WorldGenerator::CheckNeighbours, this, std::ref(pixelEmpty_copy), 0, quaterCol, 1);
			std::thread t2(&WorldGenerator::CheckNeighbours, this, std::ref(pixelEmpty_copy), quaterCol, 2 * quaterCol, 2);
			std::thread t3(&WorldGenerator::CheckNeighbours, this, std::ref(pixelEmpty_copy), 2 * quaterCol, 3 * quaterCol, 3);
			std::thread t4(&WorldGenerator::CheckNeighbours, this, std::ref(pixelEmpty_copy), 3 * quaterCol, m_windowHeight, 4);

			t1.join();
			t2.join();
			t3.join();
			t4.join();
		}	
		else
		{
			std::thread t1(&WorldGenerator::CheckNeighbours, this, std::ref(pixelEmpty_copy), 0, 2 * quaterCol, 1);
			std::thread t2(&WorldGenerator::CheckNeighbours, this, std::ref(pixelEmpty_copy), 2 * quaterCol, m_windowHeight, 2);
			t1.join();
			t2.join();
		}
		//CheckNeighbours(pixelArray_copy, 0, windowHeight, 1);
		

		m_pixelEmpty = pixelEmpty_copy;
		
		m_itterations++;
		
				afterTime = Engine::GetInstance()->GetTimeAsSeconds();
				std::cout << "Iteration " << m_itterations << " " << afterTime - prevTime << std::endl;
	}
	
	m_itterationInfo->m_textComponent->SetText("Iterations: " + std::to_string(m_itterations));
	
	ColorizeWorld(false);
}

void WorldGenerator::CheckNeighbours(std::vector<int>& pixelArray_copy, int startColumn, int endColumn, int thread)
{
	int i = startColumn * m_windowWidth;

	for (int y = startColumn; y < endColumn; y++)
	{

		for (int x = 0; x < m_windowWidth; x++)
		{
			bool* tmpNeighbours = nullptr;
			switch (thread)
			{
			case 1:
				tmpNeighbours = GetNeighbours(i);
				break;
			case 2:
				tmpNeighbours = GetNeighbours2(i);
				break;
			case 3:
				tmpNeighbours = GetNeighbours3(i);
				break;
			case 4:
				tmpNeighbours = GetNeighbours4(i);
				break;
			default:
				DebugPrint("Wrong Thread-Number set", TextColor::Red, DebugChannel::Game, __FILE__, __LINE__, true);
				return;
			}

			int counter = 0;
			for (int j = 0; j < 3; j++)
			{
				for (int k = 0; k < 3; k++)
				{
					if (tmpNeighbours[(j * 3) + k] == false) continue;

					int offset = (j - 1) * m_windowWidth + (k - 1);
					if (m_pixelEmpty[i + offset] == 0)
					{
						counter++;
					}
				}
			}
			if (counter > 4) // wall
			{
				pixelArray_copy[i] = 0;
			}
			else // empty space
			{
				pixelArray_copy[i] = 1;
			}

			i++;
		}
	}
}

int stackDepth = 0;
void WorldGenerator::ColorizeWorld(bool useColor)
{
	bool everyBackgroundHeaven = true;
	bool everyBackgroundCave = false;

	prevTime = Engine::GetInstance()->GetTimeAsSeconds();


	if (useColor)
	{
		CancleBrushMode();


		std::fill(m_isLit.begin(), m_isLit.end(), false);


		/////////////////////////////////////////////
		// Visual Pass 1 (after terrain is fully generated)

		std::vector<int> caveFloorIndices;
		int i = 0;

		for (int y = 0; y < m_windowHeight; y++)
		{
			for (int x = 0; x < m_windowWidth; x++)
			{
				if (m_pixelEmpty[i] == 0) // wall
				{
					bool* neighbours = GetNeighbours(i);
					if (neighbours[1] && m_pixelEmpty[i - m_windowWidth] == 1) // floor in cave (stone)
					{
						caveFloorIndices.push_back(i);
					}
					else // cave
					{
						bmp::Color col = m_bmpGroundTile.GetColor(x % m_tileSize.x, y % m_tileSize.y);
						m_pixelColors[i * 4 + 0] = sf::Uint8(col.r * 255);
						m_pixelColors[i * 4 + 1] = sf::Uint8(col.g * 255);
						m_pixelColors[i * 4 + 2] = sf::Uint8(col.b * 255);
					}

				}
				else // empty space
				{
					float fade = 1.0f - (float)y / m_windowHeight * 0.3f;
					if (everyBackgroundHeaven)
					{
						m_pixelColors[i * 4 + 0] = sf::Uint8(40 * fade);
						m_pixelColors[i * 4 + 1] = sf::Uint8(40 * fade);
						m_pixelColors[i * 4 + 2] = sf::Uint8(230 * fade);
					}
					else if (everyBackgroundCave)
					{
						m_pixelColors[i * 4 + 0] = sf::Uint8(35);
						m_pixelColors[i * 4 + 1] = sf::Uint8(20);
						m_pixelColors[i * 4 + 2] = sf::Uint8(5);
					}
					else
					{
						if (y < m_groundHeights[x]) //heaven
						{
							m_isLit[i] = true;
							m_pixelColors[i * 4 + 0] = sf::Uint8(40 * fade);
							m_pixelColors[i * 4 + 1] = sf::Uint8(40 * fade);
							m_pixelColors[i * 4 + 2] = sf::Uint8(230 * fade);
						}
						else // cave background
						{
							m_pixelColors[i * 4 + 0] = sf::Uint8(35);
							m_pixelColors[i * 4 + 1] = sf::Uint8(20);
							m_pixelColors[i * 4 + 2] = sf::Uint8(5);
						}
					}
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
				m_pixelColors[y * 4 + 0] = sf::Uint8(200);
				m_pixelColors[y * 4 + 1] = sf::Uint8(200);
				m_pixelColors[y * 4 + 2] = sf::Uint8(200);

				y += m_windowWidth;
				level++;

				if (y >= m_numOfPixels - 1) // window ground reached
				{
					break;
				}
			}
			while (GetNeighbours(y)[7] && level < m_caveStoneDepth);
		}

		afterTime = Engine::GetInstance()->GetTimeAsSeconds();
		std::cout << "Colorization (1st Pass): " << afterTime - prevTime << std::endl;



		/////////////////////////////////////////////
		// Visual Pass 2 

		prevTime = Engine::GetInstance()->GetTimeAsSeconds();
		for (int x = 0; x < m_windowWidth; x++)
		{
			bool groundReached = false;
			//int y = m_groundHeights[x]; // upper world edge as starting point
			int y = 0;
			int i = y * m_windowWidth + x;

			while (m_pixelEmpty[i] == 1)
			{
				i += m_windowWidth;

				if (i >= m_numOfPixels - 1) // window ground reached
				{
					groundReached = true;
					break;
				}
			}


			if (!groundReached) // grass at bottom
			{
				int level = 0;
				while (m_pixelEmpty[i] == 0 && level < m_grasDepth)
				{
					m_pixelColors[i * 4 + 0] = sf::Uint8(0);
					m_pixelColors[i * 4 + 1] = sf::Uint8(255);
					m_pixelColors[i * 4 + 2] = sf::Uint8(0);

					i += m_windowWidth;
					level++;

					if (i >= m_numOfPixels - 1) // window ground reached
					{
						break;
					}
				}
			}
		}



		afterTime = Engine::GetInstance()->GetTimeAsSeconds();
		std::cout << "Colorization (2nd Pass): " << afterTime - prevTime << std::endl;
	}

	else
	{
		sf::Uint8 color;
		int i = 0;

		for (int y = 0; y < m_windowHeight; y++)
		{
			for (int x = 0; x < m_windowWidth; x++)
			{
				if (m_pixelEmpty[i] == 1)
					color = 0;
				else
					color = 255;

				m_pixelColors[i * 4 + 0] = color;
				m_pixelColors[i * 4 + 1] = color;
				m_pixelColors[i * 4 + 2] = color;

				i++;
			}
		}
			
		afterTime = Engine::GetInstance()->GetTimeAsSeconds();
		std::cout << "Colorization (Black/White): " << afterTime - prevTime << std::endl;
	}

	m_texture.update(m_pixelColors);
	m_worldGenerationImage->GetComponent<ShaderComponent>()->m_shader.setUniform("tex", m_texture);

}

void WorldGenerator::SaveWorld()
{
	int i = 0;
	for (int y = 0; y < m_windowHeight; y++)
	{
		for (int x = 0; x < m_windowWidth; x++)
		{
			bmp::Color texColor = bmp::Color(m_pixelColors[i * 4] / 255.0f, m_pixelColors[i * 4 + 1] / 255.0f, m_pixelColors[i * 4 + 2] / 255.0f);
			m_bmpWorld.SetColor(texColor, x, y);
			i++;
		}
	}

	std::string fileName = m_inputField->GetText();
	std::ifstream inputStream(m_pathBmpIndex);
	std::string bmpIndex;
	std::getline(inputStream, bmpIndex);

	std::string savePath = "../Resources/bmp/" + fileName + ".bmp";
	m_bmpWorld.Export(savePath.c_str());

	int index = std::stoi(bmpIndex);
	if (fileName == "GeneratedMap" + bmpIndex)
	{
		std::ofstream outputStream(m_pathBmpIndex);
		index++;
		outputStream << std::to_string(index);
		outputStream.close();
	}

	m_inputField->SetText("GeneratedMap" + std::to_string(index));

}

void WorldGenerator::PushOldValues()
{
	if (m_initializing) return;

	PixelValues newValues;

	if (m_pixelValuesStack.size() >= m_MaxStackSize)
	{
		m_pixelValuesStack.pop_front();
	}

	newValues.m_pixelEmpty = m_pixelEmpty;

	m_pixelValuesStack.push_back(newValues);
}

void WorldGenerator::Undo()
{
	if (m_pixelValuesStack.size() == 0) return;
	
	PixelValues values = m_pixelValuesStack.back();
	m_pixelEmpty = values.m_pixelEmpty;

	m_pixelValuesStack.pop_back();

	ColorizeWorld(false);
}

void WorldGenerator::ChangeDensity(float relativeValue)
{
	m_density = relativeValue * 100;
	m_densityInfo->m_textComponent->SetText("Density: " + std::to_string(m_density));
}

void WorldGenerator::ChangeIterationSteps(float relativeValue)
{
	m_itterationSteps = relativeValue * 30;
	m_itterationStepsInfo->m_textComponent->SetText("Steps: " + std::to_string(m_itterationSteps));
}

void WorldGenerator::ToggleBrushMode()
{
	m_isBrushMode = !m_isBrushMode;
	m_brushTool->Activate(m_isBrushMode);

	if (m_isBrushMode)
	{
		ColorizeWorld(false);
	}
}

void WorldGenerator::CancleBrushMode()
{
	if (m_isBrushMode)
	{
		m_isBrushMode = false;
		m_brushTool->Activate(false);
	}
}

void WorldGenerator::Draw(sf::Vector2f mousePos, bool isPressedDown)
{
	if (isPressedDown)
	{
		PushOldValues();
	}

	m_brushTool->Draw(isPressedDown);
}

void WorldGenerator::Erase(sf::Vector2f mousePos, bool isPressedDown)
{
	if (isPressedDown)
	{
		PushOldValues();
	}

	m_brushTool->Erase(isPressedDown);

}

bool* WorldGenerator::GetNeighbours(int i, bool onlyLeftandRight)
{
	short edge[] = { 0,0 };

	edge[0] = 0;
	edge[1] = 0;

	// Check for Edges
	if (i % m_windowWidth == 0) // left edge
	{
		edge[0] = -1;
	}
	else if ((i + 1) % m_windowWidth == 0) // right edge
	{
		edge[0] = 1;
	}

	if (!onlyLeftandRight)
	{
		if (i < m_windowWidth) // upper edge
		{
			edge[1] = -1;
		}
		else if (i >= (m_windowWidth * m_windowHeight - m_windowWidth)) // lower edge
		{
			edge[1] = 1;
		}
	}

	// Set Neighbours
	for (int i = 0; i < 9; i++)
	{
		m_neighbours[i] = true;
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
bool* WorldGenerator::GetNeighbours2(int i, bool onlyLeftandRight)
{
	short edge[] = { 0,0 };

	edge[0] = 0;
	edge[1] = 0;

	// Check for Edges
	if (i % m_windowWidth == 0) // left edge
	{
		edge[0] = -1;
	}
	else if ((i + 1) % m_windowWidth == 0) // right edge
	{
		edge[0] = 1;
	}

	if (!onlyLeftandRight)
	{
		if (i < m_windowWidth) // upper edge
		{
			edge[1] = -1;
		}
		else if (i >= (m_windowWidth * m_windowHeight - m_windowWidth)) // lower edge
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
bool* WorldGenerator::GetNeighbours3(int i, bool onlyLeftandRight)
{
	short edge[] = { 0,0 };

	edge[0] = 0;
	edge[1] = 0;

	// Check for Edges
	if (i % m_windowWidth == 0) // left edge
	{
		edge[0] = -1;
	}
	else if ((i + 1) % m_windowWidth == 0) // right edge
	{
		edge[0] = 1;
	}

	if (!onlyLeftandRight)
	{
		if (i < m_windowWidth) // upper edge
		{
			edge[1] = -1;
		}
		else if (i >= (m_windowWidth * m_windowHeight - m_windowWidth)) // lower edge
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
bool* WorldGenerator::GetNeighbours4(int i, bool onlyLeftandRight)
{
	short edge[] = { 0,0 };

	edge[0] = 0;
	edge[1] = 0;

	// Check for Edges
	if (i % m_windowWidth == 0) // left edge
	{
		edge[0] = -1;
	}
	else if ((i + 1) % m_windowWidth == 0) // right edge
	{
		edge[0] = 1;
	}

	if (!onlyLeftandRight)
	{
		if (i < m_windowWidth) // upper edge
		{
			edge[1] = -1;
		}
		else if (i >= (m_windowWidth * m_windowHeight - m_windowWidth)) // lower edge
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

void WorldGenerator::Destroy()
{
	InputManager& inputManager = Engine::GetInstance()->GetInputManager();
	inputManager.UnregisterRectangleEntry(&m_worldGenerationImage->GetTransform());
	inputManager.UnregisterRectangleEntry(&m_worldGenerationImage->GetTransform());

	delete[] m_pixelColors;
}
