#include "../../Engine/Scene.h"
#include "../../Engine/UI_Elements/Text.h"
#include "../../Engine/UI_Elements/Button.h"
#include "../CreateLobbyForm.h"

class MultiplayerLobbyListScene : public Scene
{
public:
	void Init() override;

private:
	Text* m_headerText = nullptr;
	Text* m_infoText = nullptr;
	Button* m_createLobbyButton = nullptr;
	CreateLobbyForm* m_createLobbyForm = nullptr;
	// Form to create a lobby (own entity)
	// LobbySearcher (own entity)
	// List of LobbyListEntries (entities)
};