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
#include "../Engine/EntityComponents/SpriteComponent.h"
#include "../Engine/EntityComponents/TextComponent.h"
#include "../Engine/EntityComponents/Primitives/RectangleComponent.h"

//Entities
#include "Player.h"
#include "../Engine/UI_Elements/ButtonMenu.h"
#include "../Engine/UI_Elements/Button.h"
#include "../Engine/UI_Elements/Image.h"

//Scenes
#include "Scenes/MainMenu.h"
#include "Scenes/WorldGenerator.h"
#include "Scenes/PlayScene.h"

//Others
#include "../Engine/BMPImage.h"


void GameManager::EntityInit()
{
	InputManager& inputManager = Engine::GetInstance()->GetInputManager();

	//inputManager.RegisterKeyboardEntry(KEY::E, std::bind(&GameManager::OnInputRecieved, this, std::placeholders::_1, std::placeholders::_2));
	inputManager.RegisterKeyboardEntry(KEY::Escape, std::bind(&GameManager::OnInputRecieved, this, std::placeholders::_1, std::placeholders::_2));
	//inputManager.RegisterKeyboardEntry(KEY::F, std::bind(&GameManager::OnInputRecieved, this, std::placeholders::_1, std::placeholders::_2));

	m_currentScene = std::make_unique<WorldGenerator>();
	m_currentScene->Init();
}

void GameManager::OnInputRecieved(const KEY key, const bool keyDown)
{
	if (!keyDown) return;

	switch (key)
	{
	case KEY::E:
		SwitchScene(1);
		break;
	case KEY::Escape:
		SwitchScene(0);
		break;
	case KEY::F:
		FlushGlobalEntities();
		break;
	}
}

void GameManager::FlushGlobalEntities()
{
	EntitySystem& entitySystem = Engine::GetInstance()->GetEntitySystem();
	for (int i = 0; i < m_globalEntities.size(); i++)
	{
		entitySystem.DestroyEntity(m_globalEntities[i]);
	}
	m_globalEntities.clear();
}

void GameManager::SwitchScene(int index)
{
	m_currentScene.reset();

	switch (index)
	{
	case 0:
		m_currentScene = std::make_unique<MainMenu>();
		m_currentScene->Init();
		break;
	case 1:
		m_currentScene = std::make_unique<PlayScene>();
		m_currentScene->Init();
		break;
	case 2:
		m_currentScene = std::make_unique<WorldGenerator>();
		m_currentScene->Init();
		break;
	}
}


