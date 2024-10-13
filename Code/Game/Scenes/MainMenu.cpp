#include "MainMenu.h"

#include <functional>

#include "../../Engine/Engine.h"
#include "../../Engine/UI_Elements/ButtonMenu.h"
#include "../GameManager.h"

void MainMenu::Init()
{
	m_name = "MainMenu";

	sf::RenderWindow& window = Engine::GetInstance()->GetRenderWindow();

	ButtonMenu* buttonMenu = Instantiate(ButtonMenu, MainButtonMenu);
	assert(buttonMenu);

	std::array<KEY, 3> keys =
	{
		KEY::W,
		KEY::S,
		KEY::Enter,
	};

	std::vector<Button*> buttons = buttonMenu->InitMenu(3, keys);
	sf::Vector2f windowCenter = sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2);

	int yOffset = 100;
	buttons[0]->GetTransform().translate(windowCenter - sf::Vector2f(0, yOffset));
	buttons[1]->GetTransform().translate(windowCenter);
	buttons[2]->GetTransform().translate(windowCenter + sf::Vector2f(0, yOffset));

	buttons[0]->SetFontSize(64);
	buttons[1]->SetFontSize(64);
	buttons[2]->SetFontSize(64);

	buttons[0]->SetText("Start (not available yet)");
	buttons[1]->SetText("World-Generator");
	buttons[2]->SetText("Quit");

	//buttons[0]->SetButtonCallback([]() { dynamic_cast<GameManager*>(Engine::GetInstance()->GetGameManager())->SwitchScene(1); });
	buttons[1]->SetButtonCallback([]() { dynamic_cast<GameManager*>(Engine::GetInstance()->GetGameManager())->SwitchScene(2); });
	buttons[2]->SetButtonCallback([] { Engine::GetInstance()->GetRenderWindow().close(); });
}
