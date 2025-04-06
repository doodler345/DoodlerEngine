#include "CreateLobbyForm.h"

#include <string>

#include <SFML/Graphics.hpp>

#include "../../Engine/Engine.h"
#include "../GameManager.h"
#include "../Scenes/MultiplayerLobbyListScene.h"

void CreateLobbyForm::EntityInit()
{
	// General
	Engine* engine = Engine::GetInstance();
	sf::Vector2u windowSize = engine->GetRenderWindow().getSize();
	m_scene = reinterpret_cast<GameManager*>(Engine::GetInstance()->GetGameManagerEntity())->GetCurrentScene();

	// Background
	m_background = std::make_shared<RectangleComponent>(this, 1);
	m_background->GetRectangle()->setSize(sf::Vector2f(windowSize.x * m_RELATIVE_SIZE.x, windowSize.y * m_RELATIVE_SIZE.y));
	m_background->Center();
	m_background->GetOwner()->GetTransformable().move(windowSize.x / 2, windowSize.y / 2);
	AddComponent(m_background);

	// Buttons
	m_buttonMenu = m_scene->Instantiate(ButtonMenu, ButtonMenu);

	std::array<KEY, 3> keys =
	{
		KEY::W,
		KEY::S,
		KEY::Enter,
	};
	std::vector<Button*> buttons = m_buttonMenu->InitMenu(2, keys);
	sf::Vector2f windowCenter = sf::Vector2f(windowSize.x / 2, windowSize.y / 2);

	int yOffset = 100;
	buttons[0]->GetTransformable().move(windowSize.x * m_RELATIVE_SIZE.x, windowSize.y * m_RELATIVE_SIZE.y + ((1.0f - m_RELATIVE_SIZE.y) * windowSize.y) * 0.5f - 50);
	buttons[1]->GetTransformable().move(windowSize.x * m_RELATIVE_SIZE.x + ((1.0f - m_RELATIVE_SIZE.x) * windowSize.x) * 0.5f - 50, ((1.0f - m_RELATIVE_SIZE.y) * windowSize.y) * 0.5f + 50);

	buttons[0]->SetFontSize(64);
	buttons[1]->SetFontSize(64);

	buttons[0]->SetText("Create");
	buttons[1]->SetText("X");

	buttons[0]->SetButtonCallback(std::bind(&CreateLobbyForm::CreateLobby, this));
	buttons[1]->SetButtonCallback(std::bind(&CreateLobbyForm::SetActive, this, false));

	buttons[0]->SetRenderLayer(1);
	buttons[1]->SetRenderLayer(1);

	// Text
	m_waitForPlayerText = m_scene->Instantiate(Text, WaitForPlayerText);
	m_waitForPlayerText->m_textComponent->SetText("Waiting for player...");
	m_waitForPlayerText->m_textComponent->SetFontSize(50);
	m_waitForPlayerText->m_textComponent->SetRenderLayer(1);
	m_waitForPlayerText->GetTransformable().move(windowCenter);
	m_waitForPlayerText->m_textComponent->SetVisibility(false);
}

void CreateLobbyForm::Update(float deltaTime)
{
	if (m_isBroadcastingLobby)
	{
		BroadcastLobby();
	}
}

void CreateLobbyForm::DestroyDerived()
{
}

void CreateLobbyForm::SetActive(bool value)
{
	m_buttonMenu->SetVisibility(value);
	m_background->SetVisibility(value);
	reinterpret_cast<MultiplayerLobbyListScene*>(m_scene)->SetCreateLobbyButtonVisibility(!value);
}

void CreateLobbyForm::CreateLobby()
{
	m_buttonMenu->SetVisibility(false);
	m_waitForPlayerText->m_textComponent->SetVisibility(true);

	// Create World
	m_broadcastWorld = m_scene->Instantiate(World, GameWorld);
	std::string strPath = "../Resources/bmp/DontDelete/DefaultMap.bmp";
	const char* path = strPath.c_str();
	m_broadcastWorld->Setup(path);

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

	std::string lobbyName = "LobbyName";
	m_lobbyPacket << lobbyName;
	m_worldPacket << m_broadcastWorld->m_pixelValues;
	
	m_udpSocket.setBlocking(false);
	m_udpSocket.bind(m_PORT);
	
	m_tcpListener.setBlocking(false);
	m_tcpClientSocket.setBlocking(false);
	
	m_tcpListener.listen(m_PORT);

	m_isBroadcastingLobby = true;
}

void CreateLobbyForm::BroadcastLobby()
{
	m_udpSocket.send(m_lobbyPacket, m_broadcastIp, m_PORT);

	if (m_tcpListener.accept(m_tcpClientSocket) != sf::Socket::Done)
	{
		return;
	}

	m_tcpClientSocket.send(m_worldPacket);
}
