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


class WorldGenerator : public Scene
{
public:
	World* m_worldGenerationImage = nullptr;
	BrushTool* m_brushTool = nullptr;
	ButtonMenu* m_buttonMenu = nullptr;
	InputField* m_inputField = nullptr;
	Slider* m_sliderDensity = nullptr;
	Slider* m_sliderIterationSteps = nullptr; 
	Empty* m_sliderIterationStepsHandler = nullptr; //  TODO: Vereinfachen. Slider sollte keinen extra Handler haben?
	Empty* m_sliderDensityHandler = nullptr; // TODO: Vereinfachen. Slider sollte keinen extra Handler haben?

	Text* m_densityInfo = nullptr;
	Text* m_itterationInfo = nullptr;
	Text* m_itterationStepsInfo = nullptr;
	Text* m_brushRadiusInfo = nullptr;

private:
	virtual void Init() override;
	void OnChangeDensity(float relativeValue);
	void OnGenerateNoisemap();
	void OnChangeIterationSteps(float relativeValue);
	void OnCellularAutomataIterate();
	void OnToggleBrushMode();
	void OnUndo();
	void OnColorizeWorld();
	void OnSaveWorld();

	void CheckNeighbours(std::vector<int>& pixelArray_copy, int startColumn, int endColumn, int thread);
	void ColorizeWorld(bool useColor);
	void PushOldValues();

	void CancleBrushMode();
	void OnDraw(sf::Vector2f mousePos, bool isPressedDown);
	void OnErase(sf::Vector2f mousePos, bool isPressedDown);

	virtual void Destroy() override;

	const float m_upperWorldPercentBase =	40;

	int m_density =				65;
	int m_itterations =			0;
	int m_itterationSteps =		30;
	bool m_isBrushMode = false;

	std::list<std::vector<int>> m_pixelValuesStack;
	const int m_MaxStackSize = 30;

	std::vector<int> m_groundHeights;

	

	const char* m_pathBmpIndex = "../Resources/bmp/DontDelete/bmpIndex.txt";
	const char* m_pathBmpWorld = "../Resources/bmp/DontDelete/DontDelete.bmp";
	bool m_initializing = true;
};

