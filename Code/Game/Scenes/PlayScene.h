#pragma once

#include <string>

#include "../../Engine/Scene.h"
#include "../World.h"

class Player;

class PlayScene : public Scene
{
public:
	~PlayScene();
	void SetWorld(std::string bmpFileName);
	World* GetWorld();

	void GameOver(int deadPlayerNumber);

private:
	void Init() override;
	void ReloadScene(const KEY key, const bool keyDown);
	World* m_world = nullptr;
	std::string m_bmpFilePath;

	Player* m_player1 = nullptr;
	Player* m_player2 = nullptr;
	bool m_isGameOver = false;
};

