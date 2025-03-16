#include "LevelSelectionScene.h"

#include <array>
#include <filesystem>
#include <functional>

#include "../GameManager.h"


void LevelSelectionScene::Init()
{
	m_name = "LevelSelection";

	//Instantiations
	m_headerText = Instantiate(Text, HeaderText);
	m_buttonMenu = Instantiate(ButtonMenu, GenerateWorldButtonMenu);

	// Header Text
	m_headerText->m_textComponent->SetText("Level-Selection");
	m_headerText->m_textComponent->SetFontSize(80);
	m_headerText->GetTransform().translate(Engine::GetInstance()->GetRenderWindow().getSize().x / 2, 100);

	// List all levels
	int levelCount = 0;
	
	m_levelFilePaths.push_back("../Resources/bmp/DontDelete/DefaultMap.bmp");
	m_levelFileNames.push_back("DefaultMap.bmp");
	levelCount++;

	std::string path = "../Resources/bmp/";
	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		if (entry.path().extension() == ".bmp")
		{
			std::string fileName = entry.path().filename().string();
			m_levelFilePaths.push_back(path + fileName);
			m_levelFileNames.push_back(fileName);
			levelCount++;
		}
	}

	// Button Setup
	std::array<KEY, 3> navKeys =
	{
		KEY::Left,
		KEY::Right,
		KEY::Enter
	};

	std::vector<Button*> buttons = m_buttonMenu->InitMenu(levelCount, navKeys);
	sf::Vector2u windowSize = Engine::GetInstance()->GetRenderWindow().getSize();

	int row = 1;
	int column = 1;
	for (int i = 0; i < levelCount; i++)
	{
		buttons[i]->SetText(m_levelFileNames[i]);

		std::string filePath = m_levelFilePaths[i];
		std::function<void()> callback = [filePath]() { dynamic_cast<GameManager*>(Engine::GetInstance()->GetGameManager())->SwitchToPlayScene(filePath); };
		buttons[i]->SetButtonCallback(callback);

		if (row % 10 == 0)
		{
			column++;
			row = 1;
		}
		buttons[i]->GetTransform().translate(windowSize.x / 5 * column, 170 + 50 * row);
		row++;
	}
}
