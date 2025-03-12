#pragma once

#include <string>

#include "../../Engine/Scene.h"
#include "../World.h"

class PlayScene : public Scene
{
public:
	void SetWorld(std::string bmpFileName);
	World* GetWorld();

private:
	void Init() override;
	World* m_world = nullptr;
};

