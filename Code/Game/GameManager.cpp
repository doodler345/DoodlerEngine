#include "GameManager.h"

//libs
#include <iostream>
#include <assert.h>
#include <array>
#include <functional>
#include <time.h>

#include "../Engine/Entity/EntitySystem.h"
#include "../Engine/InputManager.h"
#include "../Engine/RenderSystem.h"

//Components
#include "../Engine/EntityComponents/Drawables/SpriteComponent.h"
#include "../Engine/EntityComponents/Drawables/TextComponent.h"
#include "../Engine/EntityComponents/Drawables/RectangleComponent.h"

//Entities
#include "Player.h"
#include "../Engine/UI_Elements/ButtonMenu.h"
#include "../Engine/UI_Elements/Button.h"
#include "../Engine/UI_Elements/Image.h"

//Scenes
#include "Scenes/MainMenu.h"
#include "Scenes/WorldGenerator.h"
#include "Scenes/LevelSelectionScene.h"
#include "Scenes/PlayScene.h"
#include "Scenes/MultiplayerLobbyListScene.h"

//Others
#include "../Engine/BMPImage.h"


void GameManager::EntityInit()
{
	InputManager& inputManager = Engine::GetInstance()->GetInputManager();
	inputManager.RegisterKeyboardEntry(KEY::Escape, std::bind(&GameManager::OnInputRecieved, this, std::placeholders::_1, std::placeholders::_2));

	SwitchScene(0);
}

void GameManager::OnInputRecieved(const KEY key, const bool keyDown)
{
	if (!keyDown) return;

	switch (key)
	{
	case KEY::Escape:
		SwitchScene(0);
		break;
	}
}

Scene* GameManager::GetCurrentScene()
{
	return m_currentScene.get();
}

void GameManager::SwitchScene(int index)
{
	if (m_currentScene != nullptr)
	{
		m_currentScene.reset();
	}

	switch (index)
	{
	case 0:
		m_currentScene = std::make_unique<MainMenu>();
		m_currentScene->Init();
		break;
	case 1:
		m_currentScene = std::make_unique<LevelSelectionScene>();
		m_currentScene->Init();
		break;
	case 2:
		m_currentScene = std::make_unique<WorldGenerator>();
		m_currentScene->Init();
		break;
	case 3:
		m_currentScene = std::make_unique<MultiplayerLobbyListScene>();
		m_currentScene->Init();
		break;
	}
}

void GameManager::SwitchToPlayScene(std::string bmpFilePath)
{
	if (m_currentScene != nullptr)
	{
		m_currentScene.reset();
	}
	m_currentScene = std::make_unique<PlayScene>();
	reinterpret_cast<PlayScene*>(m_currentScene.get())->SetWorld(bmpFilePath);
	m_currentScene->Init();
}


