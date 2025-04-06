#include "CreateLobbyForm.h"

#include <functional>
#include <cstdlib>
#include <ctime>

#include <SFML/Graphics.hpp>

#include "../../Engine/Engine.h"
#include "../../Engine/EntityComponents/Drawables/ShaderComponent.h"
#include "../GameManager.h"
#include "../Scenes/MultiplayerLobbyListScene.h"

void CreateLobbyForm::EntityInit()
{
	// General
	Engine* engine = Engine::GetInstance();
	sf::Vector2u windowSize = engine->GetRenderWindow().getSize();
	m_scene = reinterpret_cast<GameManager*>(Engine::GetInstance()->GetGameManagerEntity())->GetCurrentScene();
	std::srand(std::time({}));

	// Networking Setup
	m_udpSocket.setBlocking(false);
	m_tcpListener.setBlocking(false);
	m_tcpClientSocket.setBlocking(false);
	m_udpSocket.bind(m_port);

	// Instantiations
	m_buttonMenu = m_scene->Instantiate(ButtonMenu, ButtonMenu);
	m_waitForPlayerText = m_scene->Instantiate(Text, WaitForPlayerText);
	m_lobbyNameInputField = m_scene->Instantiate(InputField, LobbyNameInputField);
	m_levelSelection = m_scene->Instantiate(LevelSelection, LevelSelection);

	// Background
	m_background = std::make_shared<RectangleComponent>(this, 1);
	m_background->GetRectangle()->setSize(sf::Vector2f(windowSize.x * m_RELATIVE_SIZE.x, windowSize.y * m_RELATIVE_SIZE.y));
	m_background->Center();
	m_background->GetOwner()->GetTransformable().move(windowSize.x / 2, windowSize.y / 2);
	m_background->SetRenderLayer(1);
	AddComponent(m_background);

	// Buttons
	std::array<KEY, 3> keys =
	{
		KEY::W,
		KEY::S,
		KEY::Enter,
	};
	std::vector<Button*> buttons = m_buttonMenu->InitMenu(3, keys);
	sf::Vector2f windowCenter = sf::Vector2f(windowSize.x / 2, windowSize.y / 2);

	int yOffset = 100;
	buttons[0]->GetTransformable().move(windowSize.x * m_RELATIVE_SIZE.x - 100, windowSize.y * m_RELATIVE_SIZE.y + ((1.0f - m_RELATIVE_SIZE.y) * windowSize.y) * 0.5f - 50);
	buttons[1]->GetTransformable().move(windowSize.x * m_RELATIVE_SIZE.x + ((1.0f - m_RELATIVE_SIZE.x) * windowSize.x) * 0.5f - 50, ((1.0f - m_RELATIVE_SIZE.y) * windowSize.y) * 0.5f + 50);
	buttons[2]->GetTransformable().move(windowCenter.x, windowCenter.y + 40);

	buttons[0]->SetFontSize(64);
	buttons[1]->SetFontSize(64);
	buttons[2]->SetFontSize(25);

	buttons[0]->SetRenderLayer(2);
	buttons[1]->SetRenderLayer(2);
	buttons[2]->SetRenderLayer(2);

	buttons[0]->SetText("Create");
	buttons[1]->SetText("X");
	buttons[2]->SetText("Level Selection");

	buttons[0]->SetButtonCallback(std::bind(&CreateLobbyForm::CreateLobby, this));
	buttons[1]->SetButtonCallback(std::bind(&CreateLobbyForm::SetVisibility, this, false));
	buttons[2]->SetButtonCallback(std::bind(&CreateLobbyForm::ToggleSelectLevelPanel, this, true));


	// Input Field
	m_lobbyNameInputField->SetText("LobbyName");
	m_lobbyNameInputField->GetTransformable().move(windowCenter);
	m_lobbyNameInputField->SetRenderLayer(2);

	// Level Selection
	m_levelSelection->SetCallback(std::bind(&CreateLobbyForm::OnSelectLevel, this, std::placeholders::_1));
	m_levelSelection->SetActive(false);
	m_levelSelection->GetButtonMenu()->SetRenderLayer(2);

	// Text (Wait for Player)
	m_waitForPlayerText->m_textComponent->SetText("Waiting for player...");
	m_waitForPlayerText->m_textComponent->SetFontSize(50);
	m_waitForPlayerText->m_textComponent->SetRenderLayer(2);
	m_waitForPlayerText->m_textComponent->SetVisibility(false);
	m_waitForPlayerText->GetTransformable().move(windowCenter);
}

void CreateLobbyForm::Update(float deltaTime)
{
	if (m_isBroadcastingLobby)
	{
		BroadcastLobby(deltaTime);
	}
}

void CreateLobbyForm::DestroyDerived()
{
}

void CreateLobbyForm::SetVisibility(bool value)
{
	m_buttonMenu->SetVisibility(value);
	m_background->SetVisibility(value);
	m_lobbyNameInputField->SetVisibility(value);
	reinterpret_cast<MultiplayerLobbyListScene*>(m_scene)->SetCreateLobbyButtonVisibility(!value);
}

void CreateLobbyForm::ToggleSelectLevelPanel(bool value)
{
	m_levelSelection->SetActive(value);
	m_buttonMenu->SetVisibility(!value);
	m_lobbyNameInputField->SetVisibility(!value);
	reinterpret_cast<MultiplayerLobbyListScene*>(m_scene)->SetCreateLobbyButtonVisibility(!value);
}

void CreateLobbyForm::OnSelectLevel(std::string filePath)
{
	SetSelectedLevelPath(filePath);
	ToggleSelectLevelPanel(false);
}

void CreateLobbyForm::CreateLobby()
{
	m_buttonMenu->SetVisibility(false);
	m_lobbyNameInputField->SetVisibility(false);
	m_waitForPlayerText->m_textComponent->SetVisibility(true);

	// Create World
	m_world = m_scene->Instantiate(World, GameWorld);
	m_world->GetComponent<ShaderComponent>()->SetVisibility(false);
	const char* path = m_selectedWorldPath.c_str();
	m_world->Setup(path);

	// Create Broadcast address
	std::string localAddress = sf::IpAddress::getLocalAddress().toString();

	int i = localAddress.length() - 1;
	while (localAddress[i] != '.')
	{
		i--;
	}
	std::string broadcastAddress = localAddress;
	broadcastAddress.erase(i + 1, localAddress.length() - 1);
	broadcastAddress.append("255");
	m_broadcastIp = sf::IpAddress(broadcastAddress);

	// Lobby Setup
	m_lobby.id = rand(); 
	m_lobby.name = m_lobbyNameInputField->GetText();
	m_lobby.ipAddress = localAddress;
	m_lobby.port = m_port;

	m_lobbyPacket << m_lobby;
	m_worldPacket << m_world->m_pixelValues;
	
	m_tcpListener.listen(m_port);

	m_isBroadcastingLobby = true;
}

void CreateLobbyForm::BroadcastLobby(float deltaTime)
{
	m_timer += deltaTime;
	if (m_timer > m_BROADCAST_INTERVAL_SECONDS)
	{
		m_udpSocket.send(m_lobbyPacket, m_broadcastIp, m_remotePort);
		m_timer = 0.0f;
	}

	if (m_tcpListener.accept(m_tcpClientSocket) == sf::Socket::Done)
	{
		m_waitForPlayerText->m_textComponent->SetText("Player found!");
		m_tcpClientSocket.send(m_worldPacket);
	}
}
