#pragma once

#include <functional>
#include <vector>
#include <string>
#include <../../Engine/Entity/Entity.h>
#include "../../Engine/UI_Elements/ButtonMenu.h"
#include "../../Engine/UI_Elements/Button.h"

class LevelSelection : public Entity
{
public:
	void EntityInit() override;
	void SetActive(bool value) override;
	void SetCallback(std::function<void(std::string, std::string)> callback) { m_callback = callback; }
	ButtonMenu* GetButtonMenu() { return m_buttonMenu; }
	void DestroyDerived() override;

private:
	void InvokeCallback(std::string filePath, std::string fileName);

	ButtonMenu* m_buttonMenu = nullptr;
	std::vector<std::string> m_levelFilePaths;
	std::vector<std::string> m_levelFileNames;

	std::function<void(std::string, std::string)> m_callback;
};
