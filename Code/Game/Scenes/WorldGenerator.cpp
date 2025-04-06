#include "WorldGenerator.h"

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <array>
#include <functional>
#include <thread>
#include <mutex>

#include <SFML/Network.hpp>

#include "../../Engine/Engine.h"
#include "../../Engine/InputManager.h"
#include "../../Engine/UI_Elements/ButtonMenu.h"
#include "../../Engine/UI_Elements/Button.h"

#include "../GameManager.h"

float prevTime = 0;
float afterTime = 0;

WorldGenerator::~WorldGenerator()
{
	if (!Engine::GetInstance()->GetRenderWindow().isOpen()) return; //avoid issues after closing game

	DebugPrint("Destroying derived Scene " + m_name, TextColor::Blue, DebugChannel::Game, __FILE__, __LINE__);

	InputManager& inputManager = Engine::GetInstance()->GetInputManager();
	inputManager.UnregisterRectangleEntry(&m_worldGenerationImage->GetTransformable());
	inputManager.UnregisterRectangleEntry(&m_worldGenerationImage->GetTransformable());
}

void WorldGenerator::Init()
{
	///////////////////////////////////////////
	// WorldGenerator

	std::srand(time(NULL));

	m_name = "WorldGenerator";

	InputManager& inputManager = Engine::GetInstance()->GetInputManager();
	sf::RenderWindow& window = Engine::GetInstance()->GetRenderWindow();

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
	m_worldGenerationImage->Setup(window.getSize().x, window.getSize().y);
	m_groundHeights.resize(window.getSize().x);

	inputManager.RegisterRectangleEntry(
		sf::Mouse::Button::Left, &m_worldGenerationImage->GetTransformable(), m_worldGenerationImage->GetComponent<RectangleComponent>()->GetRectangle(),
		std::bind(&WorldGenerator::OnDraw, this, std::placeholders::_1, std::placeholders::_2));
	inputManager.RegisterRectangleEntry(
		sf::Mouse::Button::Right, &m_worldGenerationImage->GetTransformable(), m_worldGenerationImage->GetComponent<RectangleComponent>()->GetRectangle(),
		std::bind(&WorldGenerator::OnErase, this, std::placeholders::_1, std::placeholders::_2));


	// BrushTool Setup
	m_brushTool->Setup(m_worldGenerationImage, m_brushRadiusInfo);


	// Slider Setup
	m_sliderDensity->Setup(m_sliderDensityHandler, sf::Vector2f(100, 20), std::bind(&WorldGenerator::OnChangeDensity, this, std::placeholders::_1));
	m_sliderDensity->SetValue(0.65f);

	m_sliderIterationSteps->Setup(m_sliderIterationStepsHandler, sf::Vector2f(100, 20), std::bind(&WorldGenerator::OnChangeIterationSteps, this, std::placeholders::_1));
	m_sliderIterationSteps->SetValue(0.35f);


	// InputField Setup
	std::ifstream inputStream2(m_pathBmpIndex);
	std::string bmpIndex2;
	std::getline(inputStream2, bmpIndex2);
	if (bmpIndex2.empty())
	{
		std::ofstream outputStream(m_pathBmpIndex);
		outputStream << "0";
		bmpIndex2 = "0";
		outputStream.close();
	}

	m_inputField->SetText("GeneratedMap" + bmpIndex2);
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
	buttons[indexer++]->SetButtonCallback(std::bind(&WorldGenerator::OnGenerateNoisemap, this));
	buttons[indexer++]->SetButtonCallback(std::bind(&WorldGenerator::OnCellularAutomataIterate, this));
	buttons[indexer++]->SetButtonCallback(std::bind(&WorldGenerator::OnToggleBrushMode, this));
	buttons[indexer++]->SetButtonCallback(std::bind(&WorldGenerator::OnUndo, this));
	buttons[indexer++]->SetButtonCallback(std::bind(&WorldGenerator::OnColorizeWorld, this));
	buttons[indexer++]->SetButtonCallback(std::bind(&WorldGenerator::OnSaveWorld, this));


	// Text Setup
	m_densityInfo->m_textComponent->SetText("Density: " + std::to_string(m_density));
	m_itterationInfo->m_textComponent->SetText("Iterations: " + std::to_string(m_itterations));
	m_itterationStepsInfo->m_textComponent->SetText("Steps: " + std::to_string(m_itterationSteps));
	m_brushRadiusInfo->m_textComponent->SetText("Radius: " + std::to_string(m_brushTool->GetRadius()));

	m_densityInfo->m_textComponent->SetTextColor(sf::Color::Black);
	m_itterationInfo->m_textComponent->SetTextColor(sf::Color::Black);
	m_itterationStepsInfo->m_textComponent->SetTextColor(sf::Color::Black);
	m_brushRadiusInfo->m_textComponent->SetTextColor(sf::Color::Black);



	///////////////////////////////////////////
	// Entities LAYOUT

	float xUISettings = window.getSize().x * 0.13f;
	float yDensity = window.getSize().y * 0.2f;
	float yGenerate = window.getSize().y * 0.25f;
	float yIterationSteps = window.getSize().y * 0.35f;
	float yIterate = window.getSize().y * 0.4f;
	float yBrush = window.getSize().y * 0.6f;
	float yUndo = window.getSize().y * 0.8f;
	float ySave = window.getSize().y * 0.95f;
	float yOffset = window.getSize().y * 0.05f;

	m_worldGenerationImage->GetTransformable().move(window.getSize().x * 0.6f, window.getSize().y * 0.5f);
	m_worldGenerationImage->Scale(0.7f);

	m_inputField->GetTransformable().move(window.getSize().x * 0.5f, ySave);

	m_sliderDensity->Translate(xUISettings + 70, yDensity);
	m_sliderIterationSteps->Translate(xUISettings + 70, yIterationSteps);

	indexer = 0;
	buttons[indexer++]->GetTransformable().move(xUISettings, yGenerate);
	buttons[indexer++]->GetTransformable().move(xUISettings, yIterate + yOffset);
	buttons[indexer++]->GetTransformable().move(xUISettings, yBrush);
	buttons[indexer++]->GetTransformable().move(xUISettings - 50, yUndo);
	buttons[indexer++]->GetTransformable().move(xUISettings + 50, yUndo);
	buttons[indexer++]->GetTransformable().move(window.getSize().x * 0.5f + 150, ySave);

	m_densityInfo->GetTransformable().move(xUISettings - 50, yDensity);
	m_itterationInfo->GetTransformable().move(xUISettings, yIterate);
	m_itterationStepsInfo->GetTransformable().move(xUISettings - 50, yIterationSteps);
	m_brushRadiusInfo->GetTransformable().move(xUISettings, yBrush + yOffset * 1.3f);



	///////////////////////////////////////////
	//Finalizing
	OnGenerateNoisemap();
	m_initializing = false;
}

void WorldGenerator::OnChangeDensity(float relativeValue)
{
	m_density = relativeValue * 100;
	m_densityInfo->m_textComponent->SetText("Density: " + std::to_string(m_density));
}

void WorldGenerator::OnGenerateNoisemap()
{
	PushOldValues();

	prevTime = Engine::GetInstance()->GetTimeAsSeconds();

	// Generate Noise-Map

	int texelColor;
	int rnd;
	int i = 0;

	// set floor (y) along the x-axis
	for (int i = 0; i < m_worldGenerationImage->m_worldWidth; i++) // set base floor
	{
		m_groundHeights[i] = m_worldGenerationImage->m_worldHeight * m_upperWorldPercentBase / 100;
	}

	int numOfSins = 3;
	std::vector<int> changesInHeight(m_worldGenerationImage->m_worldWidth, 0);
	for (int i = 0; i < numOfSins; i++) // change Heigths
	{
		float rndSin = rand() % 200 + 0.0001f;
		float rndAmp = rand() % 50;

		for (int x = 0; x < m_worldGenerationImage->m_worldWidth; x++)
		{
			//in sin(x / y) * z: lower y -> higher freq, z = amp
			changesInHeight[x] += sin(float(x) / (rndSin)) * rndAmp;
			m_groundHeights[x] += changesInHeight[x];
		}
	}

	// generate Noise
	for (int y = 0; y < m_worldGenerationImage->m_worldHeight; y++)
	{
		for (int x = 0; x < m_worldGenerationImage->m_worldWidth; x++)
		{
			if (y < m_groundHeights[x]) // upper World
			{
				m_worldGenerationImage->m_pixelValues[i] = 0;
			}
			else // caves
			{
				rnd = rand() % 100;
				texelColor = rnd >= m_density ? 0 : 1;

				m_worldGenerationImage->m_pixelValues[i] = texelColor;
			}

			i++;
		}
	}

	m_itterations = 0;
	m_itterationInfo->m_textComponent->SetText("Iterations: " + std::to_string(m_itterations));

	afterTime = Engine::GetInstance()->GetTimeAsSeconds();
	//std::cout << "Generate Noise Map: " << afterTime - prevTime << std::endl;

	ColorizeWorld(false);
}

void WorldGenerator::OnChangeIterationSteps(float relativeValue)
{
	m_itterationSteps = relativeValue * 30;
	m_itterationStepsInfo->m_textComponent->SetText("Steps: " + std::to_string(m_itterationSteps));
}

void WorldGenerator::OnCellularAutomataIterate()
{
	PushOldValues();

	std::vector<int> pixelAlive_copy = m_worldGenerationImage->m_pixelValues;

	// iterate
	for (int r = 0; r < m_itterationSteps; r++)
	{
		prevTime = Engine::GetInstance()->GetTimeAsSeconds();

		int quaterRow = m_worldGenerationImage->m_worldWidth / 4.f;
		int quaterCol = m_worldGenerationImage->m_worldHeight / 4.f;


		bool useFourThreads = 0;

		if (useFourThreads)
		{
			std::thread t1(&WorldGenerator::DoCellularAutomata, this, std::ref(pixelAlive_copy), 0, quaterCol, 1);
			std::thread t2(&WorldGenerator::DoCellularAutomata, this, std::ref(pixelAlive_copy), quaterCol, 2 * quaterCol, 2);
			std::thread t3(&WorldGenerator::DoCellularAutomata, this, std::ref(pixelAlive_copy), 2 * quaterCol, 3 * quaterCol, 3);
			std::thread t4(&WorldGenerator::DoCellularAutomata, this, std::ref(pixelAlive_copy), 3 * quaterCol, m_worldGenerationImage->m_worldHeight, 4);

			t1.join();
			t2.join();
			t3.join();
			t4.join();
		}
		else
		{
			std::thread t1(&WorldGenerator::DoCellularAutomata, this, std::ref(pixelAlive_copy), 0, 2 * quaterCol, 1);
			std::thread t2(&WorldGenerator::DoCellularAutomata, this, std::ref(pixelAlive_copy), 2 * quaterCol, m_worldGenerationImage->m_worldHeight, 2);
			t1.join();
			t2.join();
		}

		m_worldGenerationImage->m_pixelValues = pixelAlive_copy;

		m_itterations++;

		afterTime = Engine::GetInstance()->GetTimeAsSeconds();
		std::cout << "Iteration " << m_itterations << " " << afterTime - prevTime << std::endl;
	}

	m_itterationInfo->m_textComponent->SetText("Iterations: " + std::to_string(m_itterations));

	ColorizeWorld(false);
}

void WorldGenerator::OnToggleBrushMode()
{
	m_isBrushMode = !m_isBrushMode;
	m_brushTool->Activate(m_isBrushMode);

	if (m_isBrushMode)
	{
		ColorizeWorld(false);
	}
}

void WorldGenerator::OnUndo()
{
	if (m_pixelValuesStack.size() == 0) return;

	m_worldGenerationImage->m_pixelValues = m_pixelValuesStack.back();

	m_pixelValuesStack.pop_back();

	ColorizeWorld(false);
}

void WorldGenerator::OnColorizeWorld()
{
	ColorizeWorld(true);
}

void WorldGenerator::OnSaveWorld()
{
	int i = 0;
	BMPImage bmpWorld = BMPImage(m_worldGenerationImage->m_worldWidth, m_worldGenerationImage->m_worldHeight);
	for (int y = 0; y < m_worldGenerationImage->m_worldHeight; y++)
	{
		for (int x = 0; x < m_worldGenerationImage->m_worldWidth; x++)
		{
			bmp::Color texColor = bmp::Color(m_worldGenerationImage->m_pixelValues[i]);
			bmpWorld.SetColor(texColor, x, y);
			i++;
		}
	}

	std::string fileName = m_inputField->GetText();
	std::ifstream inputStream(m_pathBmpIndex);
	std::string bmpIndex;
	std::getline(inputStream, bmpIndex);

	std::string savePath = "../Resources/bmp/" + fileName + ".bmp";
	bmpWorld.Export(savePath.c_str());

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


void WorldGenerator::DoCellularAutomata(std::vector<int>& pixelArray_copy, int startColumn, int endColumn, int thread)
{
	int i = startColumn * m_worldGenerationImage->m_worldWidth;

	for (int y = startColumn; y < endColumn; y++)
	{

		for (int x = 0; x < m_worldGenerationImage->m_worldWidth; x++)
		{
			bool* tmpNeighbours = nullptr;
			switch (thread)
			{
			case 1:
				tmpNeighbours = m_worldGenerationImage->GetNeighbours(i);
				break;
			case 2:
				tmpNeighbours = m_worldGenerationImage->GetNeighbours2(i);
				break;
			case 3:
				tmpNeighbours = m_worldGenerationImage->GetNeighbours3(i);
				break;
			case 4:
				tmpNeighbours = m_worldGenerationImage->GetNeighbours4(i);
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

					int offset = (j - 1) * m_worldGenerationImage->m_worldWidth + (k - 1);
					if (m_worldGenerationImage->m_pixelValues[i + offset] == 1)
					{
						counter++;
					}
				}
			}
			if (counter > 4) // wall
			{
				pixelArray_copy[i] = 1;
			}
			else // empty space
			{
				pixelArray_copy[i] = 0;
			}

			i++;
		}
	}
}


void WorldGenerator::ColorizeWorld(bool useColor)
{
	if (useColor)
	{
		CancleBrushMode();
		m_worldGenerationImage->Colorize(true);
	}
	else
	{
		m_worldGenerationImage->Colorize(false);
	}
}

void WorldGenerator::PushOldValues()
{
	if (m_initializing) return;

	if (m_pixelValuesStack.size() >= m_MaxStackSize)
	{
		m_pixelValuesStack.pop_front();
	}

	m_pixelValuesStack.push_back(m_worldGenerationImage->m_pixelValues);
}


void WorldGenerator::CancleBrushMode()
{
	if (m_isBrushMode)
	{
		m_isBrushMode = false;
		m_brushTool->Activate(false);
	}
}

void WorldGenerator::OnDraw(sf::Vector2f mousePos, bool isPressedDown)
{
	if (isPressedDown)
	{
		PushOldValues();
	}

	m_brushTool->Draw(isPressedDown);
}

void WorldGenerator::OnErase(sf::Vector2f mousePos, bool isPressedDown)
{
	if (isPressedDown)
	{
		PushOldValues();
	}

	m_brushTool->Erase(isPressedDown);
}
