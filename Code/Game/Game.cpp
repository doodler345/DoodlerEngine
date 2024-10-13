#include "../Engine/Engine.h"
#include "../Engine/RenderSystem.h"
#include "../Engine/ResourceManager.h"
#include "../Engine/Entity/EntitySystem.h"
#include "../Engine/Entity/EntityFactoryManager.h"
#include "../Engine/EntityComponents/SpriteComponent.h"
#include "../Engine/InputManager.h"

#include <SFML/Graphics.hpp>

//Entities
#include "GameManager.h"
#include "Player.h"
#include "World.h"
#include "BrushTool.h"


#include "../Engine/BMPImage.h"


int main()
{
    EntityFactoryManager* entitiyFactoryManager = EntityFactoryManager::GetInstance();
    entitiyFactoryManager->Register<GameManager>("GameManager");
    entitiyFactoryManager->Register<Player>("Player");
    entitiyFactoryManager->Register<World>("World");
    entitiyFactoryManager->Register<BrushTool>("BrushTool");

    int windowWidth = 1280;
    int windowHeight = 720;

    Engine engine;
    engine.Run(windowWidth, windowHeight);

    return 0;
}
