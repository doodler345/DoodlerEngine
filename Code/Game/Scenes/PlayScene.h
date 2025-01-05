#pragma once

#include "../../Engine/Scene.h"
#include "../World.h"

class PlayScene : public Scene
{
public:
	World* GetWorld();

private:
	void Init() override;
	World* m_world = nullptr;
};

