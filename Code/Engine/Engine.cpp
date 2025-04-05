#include "Engine.h"

#include "RenderSystem.h"
#include "ResourceManager.h"
#include "Entity/EntitySystem.h"
#include "Entity/EntityFactoryManager.h"
#include "EntityComponents/Drawables/SpriteComponent.h"
#include "InputManager.h"
#include "BMPImage.h"


//Engine-Entities
#include "Entity/Empty.h"
#include "UI_Elements/Button.h"
#include "UI_Elements/ButtonMenu.h"
#include "UI_Elements/Image.h"
#include "UI_Elements/Text.h"
#include "UI_Elements/InputField.h"
#include "UI_Elements/Slider.h"

#include "Debug.h"
#include <iostream>
#include <random>
#include <array>
#include <vector>
#include <assert.h>

Engine* Engine::m_s_instance = nullptr;

void Engine::Run(int windowWidth, int windowHeight)
{
    m_s_instance = this;

    sf::Time time;
    sf::Time prevTime = m_clock.getElapsedTime();
    float deltaTime = 0;
    float fpsUpdateTimer = 0;
    float fpsRefreshRate = 0.5f;
    float fps = 0;
    float mouseScrollDelta = 0;

    m_renderWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode(windowWidth, windowHeight), "FPS", sf::Style::Titlebar | sf::Style::Close);

    m_renderSystem = std::make_unique<RenderSystem>();
	m_renderSystem->Init(m_renderWindow.get());
    m_resourceManager = std::make_unique<ResourceManager>();
    m_entitySystem = std::make_unique<EntitySystem>();
    m_inputManager = std::make_unique<InputManager>();


    EntityFactoryManager* entityFactoryManager = EntityFactoryManager::GetInstance();
    entityFactoryManager->Register<Empty>("Empty");
    entityFactoryManager->Register<Button>("Button");
    entityFactoryManager->Register<ButtonMenu>("ButtonMenu");
    entityFactoryManager->Register<Image>("Image");
    entityFactoryManager->Register<Text>("Text");
    entityFactoryManager->Register<InputField>("InputField");
    entityFactoryManager->Register<Slider>("Slider");


    m_gameManagerEntity = entityFactoryManager->CreateEntity("GameManager");
    if (!m_gameManagerEntity)
    {
        DebugPrint("A FactoryManager called 'GameManager' is necessarry!", TextColor::Red, DebugChannel::Engine, __FILE__, __LINE__, 1);
        return;
    }
    
    while (m_renderWindow->isOpen())
    {
        mouseScrollDelta = 0;

        sf::Event event;
        while (m_renderWindow->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                m_renderWindow->close();

            else if (event.type == sf::Event::MouseWheelMoved)
                mouseScrollDelta = event.mouseWheel.delta;
        }
        
        time = m_clock.getElapsedTime();
        deltaTime = (time - prevTime).asSeconds();
        prevTime = time;
        
        fpsUpdateTimer += deltaTime;
        if (fpsUpdateTimer >= fpsRefreshRate)
        {
            fps = 1 / deltaTime;
            m_renderWindow->setTitle("FPS: " + std::to_string((int)fps));
            fpsUpdateTimer = 0;
        }


        ////////
        m_renderWindow->clear(sf::Color(255, 255, 255, 255));

        m_inputManager->Update(mouseScrollDelta * deltaTime);
        m_entitySystem->Update(deltaTime);
        m_renderSystem->Update();

        m_renderWindow->display();
        ////////
    }
    
    
}

