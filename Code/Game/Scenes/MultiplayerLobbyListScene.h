#pragma once

#include "../../Engine/Scene.h"
#include "../../Engine/UI_Elements/Text.h"
#include "../../Engine/UI_Elements/Button.h"
#include "../MultiplayerLobbyList/CreateLobbyForm.h"
#include "../MultiplayerLobbyList/LobbySearcher.h"
#include "../MultiplayerLobbyList/Lobby.h"

class MultiplayerLobbyListScene : public Scene
{
public:
	void Init() override;
	void SetCreateLobbyButtonVisibility(bool value);
	void AddActiveLobby(Lobby* lobby);
	void ClearActiveLobbies();

private:
	void ToggleCreateLobbyForm(bool value);
	void Connect();

	//unsigned short m_port = 54000;

	Text* m_headerText = nullptr;
	Text* m_infoText = nullptr;
	CreateLobbyForm* m_createLobbyForm = nullptr;
	Button* m_createLobbyButton = nullptr;
	Button* m_activeLobbyButton = nullptr; // TODO: Make vector
	Lobby* m_activeLobby = nullptr;
	LobbySearcher* m_lobbySearcher = nullptr;
};