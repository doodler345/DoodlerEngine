#pragma once

#include "../../Engine/Scene.h"

#include <vector>
#include <string>
#include "../../Engine/UI_Elements/Text.h"
#include "../LevelSelection.h"

class LevelSelectionScene : public Scene
{
private:
	void Init() override;

	Text* m_headerText = nullptr;
	LevelSelection* m_levelSelection = nullptr;
};