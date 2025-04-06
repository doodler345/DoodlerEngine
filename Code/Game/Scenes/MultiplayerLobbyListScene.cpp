#include "MultiplayerLobbyListScene.h"

#include <functional>

#include "../../Engine/EntityComponents/Drawables/RectangleComponent.h"

void MultiplayerLobbyListScene::Init()
{
	//Instantiations
	m_headerText = Instantiate(Text, HeaderText);
	m_infoText = Instantiate(Text, InfoText);
	m_createLobbyButton = Instantiate(Button, CreateLobbyButton);
	m_connectButton = Instantiate(Button, ConnectButton);
	m_createLobbyForm = Instantiate(CreateLobbyForm, CreateLobbyFormEntity);
	m_lobbySearcher = Instantiate(LobbySearcher, LobbySearch);

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

	m_connectButton->SetText("Connect");
	m_connectButton->SetFontSize(30);
	m_connectButton->GetTransformable().move(windowSize.x / 7.0f * 6, windowSize.y * 0.9f);
	m_connectButton->SetButtonCallback(std::bind(&MultiplayerLobbyListScene::Connect, this));
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

void MultiplayerLobbyListScene::Connect()
{
	sf::Packet packet;

	sf::TcpSocket socket;
	sf::IpAddress serverIP = sf::IpAddress("192.168.178.164");
	sf::Socket::Status status = socket.connect(serverIP, m_PORT, sf::Time(sf::seconds(1)));

	socket.receive(packet);
	std::vector<int> data;

	packet >> data;

	int i = 0;
	sf::Vector2u windowSize = Engine::GetInstance()->GetRenderWindow().getSize();
	BMPImage bmpWorld = BMPImage(windowSize.x, windowSize.y);
	for (int y = 0; y < windowSize.y; y++)
	{
		for (int x = 0; x < windowSize.x; x++)
		{
			bmp::Color texColor = bmp::Color(data[i]);
			bmpWorld.SetColor(texColor, x, y);
			i++;
		}
	}

	std::string savePath = "../Resources/bmp/CopiedMap.bmp";
	bmpWorld.Export(savePath.c_str());
}
