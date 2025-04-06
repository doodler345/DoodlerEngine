#include "LevelSelection.h"

#include <filesystem>

#include "../Engine/Engine.h"
#include "../Engine/Scene.h"
#include "../Game/GameManager.h"

void LevelSelection::EntityInit()
{
	Scene* scene = reinterpret_cast<GameManager*>(Engine::GetInstance()->GetGameManagerEntity())->GetCurrentScene();
	m_buttonMenu = scene->Instantiate(ButtonMenu, GenerateWorldButtonMenu);

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
		std::string& fileName = m_levelFileNames[i];
		std::string& filePath = m_levelFilePaths[i];

		buttons[i]->SetText(fileName);
		//std::function<void()> callback = [filePath]() { dynamic_cast<GameManager*>(Engine::GetInstance()->GetGameManagerEntity())->SwitchToPlayScene(filePath); };
		//buttons[i]->SetButtonCallback(callback);

		std::function<void()> callback = std::bind(&LevelSelection::InvokeCallback, this, filePath, fileName);
		buttons[i]->SetButtonCallback(callback);

		if (row % 10 == 0)
		{
			column++;
			row = 1;
		}
		buttons[i]->GetTransformable().move(windowSize.x / 5 * column, 170 + 50 * row);
		row++;
	}
}

void LevelSelection::SetActive(bool value)
{
	m_buttonMenu->SetActive(value);
}

void LevelSelection::DestroyDerived()
{
}

void LevelSelection::InvokeCallback(std::string filePath, std::string fileName)
{
	m_callback(filePath, fileName);
}
