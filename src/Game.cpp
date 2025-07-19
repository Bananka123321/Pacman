#include "Controller.h"
#include "Game.h"
#include "Maps.h"
#include <iostream>
#include <windows.h>
#include <cstdlib>

int CurrentMap = -1;
std::vector<std::string> Level;

bool ChoiceUser()
{
    bool choice = 0; // Выбор пользователя 0 - PLAY, 1 - EXIT
    while(!(GetAsyncKeyState(VK_RETURN) & 0x8000)) // Выход из цикла на ESC, ENTER
    {
        GoToxy(0, choice);
        std::cout << ">";
        Sleep(100);
        GoToxy(0, choice);
        std::cout << " ";
        
        if ((GetAsyncKeyState(VK_UP) & 0x8000) || (GetAsyncKeyState(VK_DOWN) & 0x8000)) 
            choice = !choice;
    }
    return choice;                      
}

int Menu()
{       
    GoToxy(0, 0);
    std::cout << "  PLAY";
    GoToxy(0, 1);

    std::cout << "  EXIT";
    if (ChoiceUser())
        exit(0);

    Sleep(100);
    system("cls");
    
    GoToxy(0, 0);
    std::cout << "  ONE PLAYER";
    GoToxy(0, 1);
    std::cout << "  TWO PLAYERS";
    
    bool TwoPlayers = ChoiceUser();
    system("cls");
    return TwoPlayers ? 2 : 1;
}

void LoadMap()
{
    if (CurrentMap >= 0 && CurrentMap < Maps.size()) {
        Level = Maps[CurrentMap];

        offsetX = (GetConsoleWidth() - Level[0].size()) / 2;
        offsetY = (GetConsoleHeight() - Level.size()) / 2;

        for (int y = 0; y < Level.size(); y++) {
            for (int x = 0; x < Level[y].size(); x++) {
                GoToxy(x, y);
                std::cout << Level[y][x];
            }
        }
    }
}


void DrawFruitDescriptions()//Пишет описание фруктов
{
    int startX = 60;//Правее карты
    int y = 1;

    offsetX = (GetConsoleWidth() - Level[0].size()) / 2;
    offsetY = (GetConsoleHeight() - Level.size()) / 2;
    
    std::vector<std::pair<std::string, std::string>> fruits = {
        {"Cherry", "Gives +50 points"},
        {"Strawberry", "Gives +100 points"},
        {"Orange", "Speeds you up briefly"},
        {"Apple", "Slows ghosts temporarily"},
        {"Grapes", "Bonus +200 points"},
        {"Watermelon", "Extra life!"}
    };

    for (auto& [fruit, desc] : fruits)
    {
        GoToxy(startX, y++);
        std::cout << fruit;
        GoToxy(startX + 2, y);
        std::cout << desc;
        y++;
    }
}
