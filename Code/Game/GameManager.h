#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "../Engine/Scene.h"
#include "../Engine/Entity/Entity.h"
#include "../Engine/Entity/EntityFactoryManager.h"

class GameManager : public Entity
{
public:
	virtual void EntityInit() override;
	void SwitchScene(int index);
	void OnInputRecieved(const KEY key, const bool keyDown);

private:
	virtual void DestroyDerived() {}; //***** anything to put in here?
	void FlushGlobalEntities();
	std::vector<Entity*> m_globalEntities;
	std::unique_ptr<Scene> m_currentScene;
};
