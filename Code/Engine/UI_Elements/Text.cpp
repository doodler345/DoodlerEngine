#include "Text.h"


void Text::EntityInit()
{
	std::string path = "../Resources/Viga-Regular.ttf";
	std::string text = "default-text";
	std::shared_ptr<TextComponent> textComponent = std::make_shared<TextComponent>(path, text, this);
	AddComponent(textComponent);
	this->m_textComponent = textComponent.get();
}
