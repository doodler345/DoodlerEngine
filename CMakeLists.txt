cmake_minimum_required(VERSION 3.30)

set(ENGINE_PROJECT_NAME "Engine")
set(GAME_PROJECT_NAME "Game")
set(CMAKE_CXX_STANDARD 20)

project(${GAME_PROJECT_NAME})
project(${ENGINE_PROJECT_NAME})

set(THIRDPARTY_DIR "Code/ThirdParty")

# SFML
set(SFML_DIR "${CMAKE_CURRENT_LIST_DIR}/${THIRDPARTY_DIR}/SFML-2.6.1/lib/cmake/SFML")
set(OPENAL_DLL "${CMAKE_CURRENT_LIST_DIR}/${THIRDPARTY_DIR}/SFML-2.6.1/bin/openal32.dll")
set(SFML_STATIC_LIBRARIES TRUE)

find_package(SFML 2.6 COMPONENTS graphics audio network REQUIRED)
include_directories(${CMAKE_CURRENT_LIST_DIR}/${THIRDPARTY_DIR}/SFML-2.6.1/include)
include_directories(${CMAKE_CURRENT_LIST_DIR}/${THIRDPARTY_DIR}/SFMLMath)


set(ENGINE_SOURCE_PATH Code/${ENGINE_PROJECT_NAME})
set(ENGINE_SOURCE_FILES
${ENGINE_SOURCE_PATH}/BMPImage.h
${ENGINE_SOURCE_PATH}/BMPImage.cpp
${ENGINE_SOURCE_PATH}/Debug.h
${ENGINE_SOURCE_PATH}/Debug.cpp
${ENGINE_SOURCE_PATH}/Engine.h
${ENGINE_SOURCE_PATH}/Engine.cpp
${ENGINE_SOURCE_PATH}/InputManager.h
${ENGINE_SOURCE_PATH}/InputManager.cpp
${ENGINE_SOURCE_PATH}/RenderSystem.h
${ENGINE_SOURCE_PATH}/RenderSystem.cpp
${ENGINE_SOURCE_PATH}/ResourceManager.h
${ENGINE_SOURCE_PATH}/ResourceManager.cpp
${ENGINE_SOURCE_PATH}/Scene.h
${ENGINE_SOURCE_PATH}/Scene.cpp
${ENGINE_SOURCE_PATH}/Entity/Empty.h
${ENGINE_SOURCE_PATH}/Entity/Empty.cpp
${ENGINE_SOURCE_PATH}/Entity/Entity.h
${ENGINE_SOURCE_PATH}/Entity/Entity.cpp
${ENGINE_SOURCE_PATH}/Entity/EntityComponent.h
${ENGINE_SOURCE_PATH}/Entity/EntityComponent.cpp
${ENGINE_SOURCE_PATH}/Entity/EntityFactoryManager.h
${ENGINE_SOURCE_PATH}/Entity/EntityFactoryManager.cpp
${ENGINE_SOURCE_PATH}/Entity/EntitySystem.h
${ENGINE_SOURCE_PATH}/Entity/EntitySystem.cpp
${ENGINE_SOURCE_PATH}/Entity/InteractableUIEntity.h
${ENGINE_SOURCE_PATH}/Entity/InteractableUIEntity.cpp
${ENGINE_SOURCE_PATH}/EntityComponents/AnimatorComponent.h
${ENGINE_SOURCE_PATH}/EntityComponents/AnimatorComponent.cpp
${ENGINE_SOURCE_PATH}/EntityComponents/Drawables/ShaderComponent.h
${ENGINE_SOURCE_PATH}/EntityComponents/Drawables/ShaderComponent.cpp
${ENGINE_SOURCE_PATH}/EntityComponents/Drawables/SpriteComponent.h
${ENGINE_SOURCE_PATH}/EntityComponents/Drawables/SpriteComponent.cpp
${ENGINE_SOURCE_PATH}/EntityComponents/Drawables/TextComponent.h
${ENGINE_SOURCE_PATH}/EntityComponents/Drawables/TextComponent.cpp
${ENGINE_SOURCE_PATH}/EntityComponents/Drawables/CircleComponent.h
${ENGINE_SOURCE_PATH}/EntityComponents/Drawables/CircleComponent.cpp
${ENGINE_SOURCE_PATH}/EntityComponents/Drawables/RectangleComponent.h
${ENGINE_SOURCE_PATH}/EntityComponents/Drawables/RectangleComponent.cpp
${ENGINE_SOURCE_PATH}/UI_Elements/Button.h
${ENGINE_SOURCE_PATH}/UI_Elements/Button.cpp
${ENGINE_SOURCE_PATH}/UI_Elements/ButtonMenu.h
${ENGINE_SOURCE_PATH}/UI_Elements/ButtonMenu.cpp
${ENGINE_SOURCE_PATH}/UI_Elements/Image.h
${ENGINE_SOURCE_PATH}/UI_Elements/Image.cpp
${ENGINE_SOURCE_PATH}/UI_Elements/InputField.h
${ENGINE_SOURCE_PATH}/UI_Elements/InputField.cpp
${ENGINE_SOURCE_PATH}/UI_Elements/InputFieldValidKeys.h
${ENGINE_SOURCE_PATH}/UI_Elements/Slider.h
${ENGINE_SOURCE_PATH}/UI_Elements/Slider.cpp
${ENGINE_SOURCE_PATH}/UI_Elements/Text.h
${ENGINE_SOURCE_PATH}/UI_Elements/Text.cpp
)

set(GAME_SOURCE_PATH Code/${GAME_PROJECT_NAME})
set(GAME_SOURCE_FILES 
${GAME_SOURCE_PATH}/Game.cpp
${GAME_SOURCE_PATH}/BrushTool.h
${GAME_SOURCE_PATH}/BrushTool.cpp
${GAME_SOURCE_PATH}/GameManager.h
${GAME_SOURCE_PATH}/GameManager.cpp
${GAME_SOURCE_PATH}/Player.h
${GAME_SOURCE_PATH}/Player.cpp
${GAME_SOURCE_PATH}/World.h
${GAME_SOURCE_PATH}/World.cpp
${GAME_SOURCE_PATH}/Bazooka.h
${GAME_SOURCE_PATH}/Bazooka.cpp
${GAME_SOURCE_PATH}/BazookaRocket.h
${GAME_SOURCE_PATH}/BazookaRocket.cpp
${GAME_SOURCE_PATH}/MultiplayerLobbyList/Lobby.h
${GAME_SOURCE_PATH}/MultiplayerLobbyList/CreateLobbyForm.h
${GAME_SOURCE_PATH}/MultiplayerLobbyList/CreateLobbyForm.cpp
${GAME_SOURCE_PATH}/MultiplayerLobbyList/LobbySearcher.h
${GAME_SOURCE_PATH}/MultiplayerLobbyList/LobbySearcher.cpp
${GAME_SOURCE_PATH}/Scenes/MainMenu.h
${GAME_SOURCE_PATH}/Scenes/MainMenu.cpp
${GAME_SOURCE_PATH}/Scenes/LevelSelectionScene.h
${GAME_SOURCE_PATH}/Scenes/LevelSelectionScene.cpp
${GAME_SOURCE_PATH}/Scenes/PlayScene.h
${GAME_SOURCE_PATH}/Scenes/PlayScene.cpp
${GAME_SOURCE_PATH}/Scenes/MultiplayerLobbyListScene.h
${GAME_SOURCE_PATH}/Scenes/MultiplayerLobbyListScene.cpp
${GAME_SOURCE_PATH}/Scenes/WorldGenerator.h
${GAME_SOURCE_PATH}/Scenes/WorldGenerator.cpp
)


# Eigene Projekte
add_library(${ENGINE_PROJECT_NAME} ${ENGINE_SOURCE_FILES})
add_executable(${GAME_PROJECT_NAME} ${GAME_SOURCE_FILES})

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${GAME_PROJECT_NAME})

# Abhängigkeitsreihenfolge 
add_dependencies(${GAME_PROJECT_NAME} ${ENGINE_PROJECT_NAME})

# Library Verlinkungen
target_link_libraries(${ENGINE_PROJECT_NAME}
    #debug box2dd.lib 	sfml-graphics # Debug-Version
    #optimized box2d.lib sfml-graphics # Release-Version
	sfml-graphics
	sfml-audio
	sfml-network
)
target_link_libraries(${GAME_PROJECT_NAME} ${ENGINE_PROJECT_NAME})



# Copy openal.dll (audio-library) to the output directory
add_custom_command(TARGET ${GAME_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${OPENAL_DLL} $<TARGET_FILE_DIR:${GAME_PROJECT_NAME}>)