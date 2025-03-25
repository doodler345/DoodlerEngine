#include "PlayScene.h"

#include <functional>
#include <SFML/Network.hpp>

#include "../Player.h"
#include "../GameManager.h"
#include "../../Engine/Engine.h"
#include "../../Engine/InputManager.h"
#include "../../Engine/UI_Elements/Text.h"

PlayScene::~PlayScene()
{
	if (m_isGameOver)
	{
		InputManager& inputManager = Engine::GetInstance()->GetInputManager();
		inputManager.UnregisterKeyboardEntry(KEY::Z);
	}
}

void PlayScene::SetWorld(std::string bmpFilePath)
{
	m_bmpFilePath = bmpFilePath;

	//Create World Entity
	sf::RenderWindow& window = Engine::GetInstance()->GetRenderWindow();

	World* world = Instantiate(World, GameWorld);
	assert(world);

	std::string strPath = bmpFilePath;
	const char* path = strPath.c_str();
	world->Setup(path);

	world->GetTransformable().move(window.getSize().x / 2, window.getSize().y / 2);
	world->UpdateTexture();
	world->Colorize(true);
	m_world = world;

	////////////////////////////////////////////////////////
	//unsigned short port = 54000;

	//sf::Packet packet;
	//packet << world->m_pixelValues;

	//sf::TcpListener listener;
	//listener.listen(port);

	//sf::TcpSocket clientSocket;
	//listener.accept(clientSocket);

	//clientSocket.send(packet);
	////////////////////////////////////////////////////////
}

World* PlayScene::GetWorld()
{
	return m_world;
}

void PlayScene::GameOver(int deadPlayerNumber)
{
	m_isGameOver = true;

	m_player1->StopUpdate();
	m_player2->StopUpdate();

	InputManager& inputManager = Engine::GetInstance()->GetInputManager();
	inputManager.RegisterKeyboardEntry(KEY::Z, std::bind(&PlayScene::ReloadScene, this, std::placeholders::_1, std::placeholders::_2));
		

	Text* text_gameOver = Instantiate(Text, GameOverText);
	Text* text_replayButton = Instantiate(Text, GameOverReplayButtonText);
	assert(text_gameOver);
	assert(text_replayButton);

	text_gameOver->m_textComponent->SetText("Player " + std::to_string(deadPlayerNumber) + " died!");
	text_gameOver->m_textComponent->SetFontSize(75);
	text_gameOver->m_textComponent->SetTextColor(sf::Color::White);

	text_replayButton->m_textComponent->SetText("Press Z to restart");
	text_replayButton->m_textComponent->SetFontSize(50);
	text_replayButton->m_textComponent->SetTextColor(sf::Color::White);

	sf::Vector2u windowSize = Engine::GetInstance()->GetRenderWindow().getSize();
	text_gameOver->GetTransformable().move(sf::Vector2f(windowSize.x / 2, windowSize.y / 5));
	text_replayButton->GetTransformable().move(sf::Vector2f(windowSize.x / 2, windowSize.y / 5 + 70));
}

void PlayScene::Init()
{
	m_name = "PlayScene";

	//Create Game-Entities
	sf::RenderWindow& window = Engine::GetInstance()->GetRenderWindow();

	//Player 1
	m_player1 = Instantiate(Player, PlayerOne);
	assert(m_player1);

	std::array<KEY, 2> movementKeysPlayer1 =
	{
		KEY::A,
		KEY::D,
	};
	std::array<KEY, 2> aimKeysPlayer1 =
	{
		KEY::W,
		KEY::S,
	};

	m_player1->Setup(1, movementKeysPlayer1, aimKeysPlayer1, KEY::Space);
	m_player1->GetTransformable().move(sf::Vector2f(window.getSize().x / 5, window.getSize().y / 5));


	//Player 2
	m_player2 = Instantiate(Player, PlayerTwo);
	assert(m_player2);

	std::array<KEY, 2> movementKeysPlayer2 =
	{
		KEY::Left,
		KEY::Right,
	};
	std::array<KEY, 2> aimKeysPlayer2 =
	{
		KEY::Up,
		KEY::Down,
	};

	m_player2->Setup(2, movementKeysPlayer2, aimKeysPlayer2, KEY::Enter);
	m_player2->GetTransformable().move(sf::Vector2f(window.getSize().x - window.getSize().x / 5, window.getSize().y / 5));
}

void PlayScene::ReloadScene(const KEY key, const bool keyDown)
{
	if (!keyDown) return;

	dynamic_cast<GameManager*>(Engine::GetInstance()->GetGameManagerEntity())->SwitchToPlayScene(m_bmpFilePath);
}
