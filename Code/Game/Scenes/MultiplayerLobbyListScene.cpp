#include "MultiplayerLobbyListScene.h"

#include <functional>

#include "../../Engine/EntityComponents/Drawables/RectangleComponent.h"

void MultiplayerLobbyListScene::Init()
{
	//Instantiations
	m_headerText = Instantiate(Text, HeaderText);
	m_infoText = Instantiate(Text, InfoText);
	m_createLobbyButton = Instantiate(Button, CreateLobbyButton);
	m_createLobbyForm = Instantiate(CreateLobbyForm, CreateLobbyForm);

	// Text Setup
	sf::Vector2u windowSize = Engine::GetInstance()->GetRenderWindow().getSize();
	m_headerText->m_textComponent->SetText("Current Lobby-List");
	m_headerText->m_textComponent->SetFontSize(80);
	m_headerText->GetTransformable().move(windowSize.x / 2.0f, windowSize.y / 6.0f);

	m_infoText->m_textComponent->SetText("Make sure you're in the same local network. Disable VPN.");
	m_infoText->m_textComponent->SetFontSize(20);
	m_infoText->GetTransformable().move(windowSize.x / 2.0f, windowSize.y / 6.0f + 60);

	// Create Lobby Form Setup
	m_createLobbyForm->SetActive(false);
	
	// Button Setup
	m_createLobbyButton->SetText("Create Lobby");
	m_createLobbyButton->SetFontSize(30);
	m_createLobbyButton->GetTransformable().move(windowSize.x / 7.0f, windowSize.y * 0.9f);
	m_createLobbyButton->SetButtonCallback(std::bind(&MultiplayerLobbyListScene::ToggleCreateLobbyForm, this, true));
}

void MultiplayerLobbyListScene::SetCreateLobbyButtonVisibility(bool value)
{
	m_createLobbyButton->Select(value);
	m_createLobbyButton->SetVisibility(value);
}

void MultiplayerLobbyListScene::ToggleCreateLobbyForm(bool value)
{
	SetCreateLobbyButtonVisibility(!value);
	m_createLobbyForm->SetActive(true);
}
