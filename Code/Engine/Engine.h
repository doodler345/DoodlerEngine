#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
//#include <SFMLMath/SFMLMath.hpp>

class RenderSystem;
class ResourceManager;
class EntitySystem;
class InputManager;
class Entity;

namespace sf
{
	class RenderWindow;
}


class Engine
{
public:
	static Engine* GetInstance()
	{
		return m_s_instance;
	}

	void Run(int windowWidth, int windowHeight);

	sf::RenderWindow& GetRenderWindow() { return *m_renderWindow; }
	RenderSystem& GetRenderSystem() { return *m_renderSystem; }
	ResourceManager& GetResourceManager() { return *m_resourceManager; }
	EntitySystem& GetEntitySystem() { return *m_entitySystem; }
	InputManager& GetInputManager() { return *m_inputManager; }

	Entity* GetGameManager() { return m_gameManagerEntity; }

	float GetTimeAsSeconds() { return m_clock.getElapsedTime().asSeconds(); }

private:
	static Engine* m_s_instance;
	std::unique_ptr<sf::RenderWindow> m_renderWindow;
	std::unique_ptr<RenderSystem> m_renderSystem;
	std::unique_ptr<ResourceManager> m_resourceManager;
	std::unique_ptr<EntitySystem> m_entitySystem;
	std::unique_ptr<InputManager> m_inputManager;
	Entity* m_gameManagerEntity = nullptr;

	sf::Clock m_clock;
};
