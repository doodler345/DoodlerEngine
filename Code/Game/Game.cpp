#include "../Engine/Engine.h"
#include "../Engine/RenderSystem.h"
#include "../Engine/ResourceManager.h"
#include "../Engine/Entity/EntitySystem.h"
#include "../Engine/Entity/EntityFactoryManager.h"
#include "../Engine/EntityComponents/Drawables/SpriteComponent.h"
#include "../Engine/InputManager.h"

#include <SFML/Graphics.hpp>

//Entities
#include "GameManager.h"
#include "Player.h"
#include "World.h"
#include "BrushTool.h"
#include "CreateLobbyForm.h"


#include "../Engine/BMPImage.h"


int main()
{
    EntityFactoryManager* entitiyFactoryManager = EntityFactoryManager::GetInstance();
    entitiyFactoryManager->REGISTER(GameManager);
    entitiyFactoryManager->REGISTER(Player);
    entitiyFactoryManager->REGISTER(World);
    entitiyFactoryManager->REGISTER(Bazooka);
    entitiyFactoryManager->REGISTER(BazookaRocket);
    entitiyFactoryManager->REGISTER(BrushTool);
    entitiyFactoryManager->REGISTER(CreateLobbyForm);

    int windowWidth = 1280;
    int windowHeight = 720;

    Engine engine;
    engine.Run(windowWidth, windowHeight);

    return 0;
}
