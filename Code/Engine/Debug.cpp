#include "Debug.h"
#include <windows.h>
#include <filesystem>


void DebugPrint(std::string text, TextColor textColor, DebugChannel channel, std::string file, int line, bool isWarning)
{
#if SHOW_ONLY_WARNINGS
    if (!isWarning) return;
#endif

    if (isWarning || DebugVisibility[channel] == 1)
    {
        int value = static_cast<int>(textColor);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), value);
        
        if (file != "")
        {
            std::string info = std::filesystem::path(file).filename().string(); // ***** make accessible for older C++ Versions (This is C++17)
            
            if (line != -1)
            {
                info += " (l." + std::to_string(line) + ")";
            }
            
            std::cout << info << ": " << text << std::endl;
        }
        else
        {
            std::cout << text << std::endl;
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    }
}

