#pragma once

#include "../../Engine/Scene.h"

#include <vector>
#include <string>
#include "../../Engine/UI_Elements/Text.h"
#include "../../Engine/UI_Elements/ButtonMenu.h"
#include "../../Engine/UI_Elements/Button.h"

class LevelSelectionScene : public Scene
{
private:
	void Init() override;

	Text* m_headerText = nullptr;
	ButtonMenu* m_buttonMenu = nullptr;
	std::vector<std::string> m_levelFilePaths;
	std::vector<std::string> m_levelFileNames;
};