#pragma once
#include <iostream>
#include <map>


#define SHOW_ONLY_WARNINGS 0

enum class DebugChannel
{
    Engine,
    Game,
    Entity,
    EntityComponent,
    ResourceManager,
    RenderSystem,
    InputManager,
    UI
};

static std::map<DebugChannel, bool> DebugVisibility
{
    { DebugChannel::Engine,                 0 },
    { DebugChannel::Game,                   0 },
    { DebugChannel::Entity,                 0 },
    { DebugChannel::EntityComponent,        0 },
    { DebugChannel::ResourceManager,        0 },
    { DebugChannel::RenderSystem,           0 },
    { DebugChannel::InputManager,           1 },
    { DebugChannel::UI,                     0 },
};

enum class TextColor;

void DebugPrint(std::string text, TextColor, DebugChannel channel, std::string file = "", int line = -1, bool isWarning = false);

enum class TextColor
{
    Black,
    Blue,
    Green,
    Aqua,
    Red,
    Purple,
    Yellow,
    White,
    Gray,
    LightBlue,
    LightGreen,
    LightAqua,
    LightRed,
    LightPurple,
    LightYellow,
    BrightWhite
};