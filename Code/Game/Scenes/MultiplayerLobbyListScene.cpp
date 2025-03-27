#include "MultiplayerLobbyListScene.h"

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
	m_headerText->GetTransformable().move(windowSize.x / 2, windowSize.y / 6);

	m_infoText->m_textComponent->SetText("Make sure you're in the same local network. Disable VPN.");
	m_infoText->m_textComponent->SetFontSize(20);
	m_infoText->GetTransformable().move(windowSize.x / 2, windowSize.y / 6 + 60);

	// Button Setup
	m_createLobbyButton->SetText("Create Lobby");
	m_createLobbyButton->SetFontSize(30);
	m_createLobbyButton->GetTransformable().move(windowSize.x / 7, windowSize.y * 0.9f);
}
