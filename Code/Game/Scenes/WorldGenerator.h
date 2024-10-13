#pragma once

#include <list>
#include <vector>
#include <memory>
#include "../../Engine/Scene.h"
#include "../../Engine/Entity/Empty.h"
#include "../../Engine/UI_Elements/Text.h"
#include "../../Engine/UI_Elements/InputField.h"
#include "../../Engine/UI_Elements/Slider.h"
#include "../../Engine/BMPImage.h"
#include "../World.h"
#include "../BrushTool.h"


struct PixelValues
{
	std::vector<int> m_pixelEmpty;
};

class WorldGenerator : public Scene
{
public:
	World* m_worldGenerationImage = nullptr;
	BrushTool* m_brushTool = nullptr;
	ButtonMenu* m_buttonMenu = nullptr;
	InputField* m_inputField = nullptr;
	Slider* m_sliderDensity = nullptr;
	Slider* m_sliderIterationSteps = nullptr; 
	Empty* m_sliderIterationStepsHandler = nullptr; //  TODO: das ist völlig behindert
	Empty* m_sliderDensityHandler = nullptr; // TODO: das ist völlig behindert

	sf::Uint8* m_pixelColors = nullptr;
	std::vector<int> m_pixelEmpty;

	sf::Texture m_texture;
	int m_windowWidth;
	int m_windowHeight;
	int m_numOfPixels;

	Text* m_densityInfo = nullptr;
	Text* m_itterationInfo = nullptr;
	Text* m_itterationStepsInfo = nullptr;
	Text* m_brushRadiusInfo = nullptr;

private:
	virtual void Init() override;
	void GenerateNoisemap();
	void CellularAutomataIteration();
	void CheckNeighbours(std::vector<int>& pixelArray_copy, int startColumn, int endColumn, int thread);
	void ColorizeWorld(bool useColor);
	void SaveWorld();
	void PushOldValues();
	void Undo();
	void ChangeDensity(float relativeValue);
	void ChangeIterationSteps(float relativeValue);

	void ToggleBrushMode();
	void CancleBrushMode();
	void Draw(sf::Vector2f mousePos, bool isPressedDown);
	void Erase(sf::Vector2f mousePos, bool isPressedDown);

	bool* GetNeighbours (int index, bool onlyLeftandRight = false); //TODO: add onlyUpAndDown (e.g. for gras)
	bool* GetNeighbours2(int index, bool onlyLeftandRight = false); 
	bool* GetNeighbours3(int index, bool onlyLeftandRight = false); 
	bool* GetNeighbours4(int index, bool onlyLeftandRight = false); 
	virtual void Destroy() override;

	std::vector<int> m_groundHeights;
	const float m_upperWorldPercentBase =	40;
	const int m_grasDepth =					2;
	const int m_caveStoneDepth =			3;

	int m_density =				65;
	int m_itterations =			0;
	int m_itterationSteps =		30;
	bool m_isBrushMode = false;

	std::list<PixelValues> m_pixelValuesStack;
	const int m_MaxStackSize = 30;

	std::vector<bool> m_isLit = std::vector<bool>(false);
	bool m_neighbours [9];
	bool m_neighbours2[9];
	bool m_neighbours3[9];
	bool m_neighbours4[9];

	BMPImage m_bmpWorld = BMPImage(0,0);
	BMPImage m_bmpGroundTile = BMPImage(0,0);
	bmp::Vector2 m_tileSize;

	const char* m_pathBmpIndex = "../Resources/bmp/bmpIndex.txt";
	const char* m_pathBmpWorld = "../Resources/bmp/DontDelete/DontDelete.bmp";
	const char* m_pathBmpGroundTile = "../Resources/bmp/DontDelete/GroundTile.bmp";
	bool m_initializing = true;
};

