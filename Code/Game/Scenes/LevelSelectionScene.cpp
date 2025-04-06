#include "LevelSelectionScene.h"

#include <functional>

#include "../GameManager.h"


void LevelSelectionScene::Init()
{
	m_name = "LevelSelection";

	//Instantiations
	m_headerText = Instantiate(Text, HeaderText);
	m_levelSelection = Instantiate(LevelSelection, LevelSelection);

	// Header Text
	m_headerText->m_textComponent->SetText("Level-Selection");
	m_headerText->m_textComponent->SetFontSize(80);
	m_headerText->GetTransformable().move(Engine::GetInstance()->GetRenderWindow().getSize().x / 2, 100);

	// Level Selection
	m_levelSelection->SetCallback(std::bind(&GameManager::SwitchToPlayScene, reinterpret_cast<GameManager*>(Engine::GetInstance()->GetGameManagerEntity()), std::placeholders::_1));
}
