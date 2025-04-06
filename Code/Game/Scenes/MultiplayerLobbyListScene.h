#pragma once

#include "../../Engine/Scene.h"
#include "../../Engine/UI_Elements/Text.h"
#include "../../Engine/UI_Elements/Button.h"
#include "../MultiplayerLobbyList/CreateLobbyForm.h"
#include "../MultiplayerLobbyList/LobbySearcher.h"

class MultiplayerLobbyListScene : public Scene
{
public:
	void Init() override;
	void SetCreateLobbyButtonVisibility(bool value);

private:
	void ToggleCreateLobbyForm(bool value);
	void Connect();

	//unsigned short m_port = 54000;

	Text* m_headerText = nullptr;
	Text* m_infoText = nullptr;
	Button* m_createLobbyButton = nullptr;
	Button* m_connectButton = nullptr;
	CreateLobbyForm* m_createLobbyForm = nullptr;
	LobbySearcher* m_lobbySearcher = nullptr;
};