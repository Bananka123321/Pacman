#include "Controller.h"
#include "Game.h"
#include "Maps.h"
#include <iostream>
#include <windows.h>
#include <cstdlib>

int CurrentMap = -1;
std::vector<std::string> Level;

bool choice_user()
{
    bool choice = 0; // Выбор пользователя 0 - PLAY, 1 - EXIT
    while(!(GetAsyncKeyState(VK_RETURN) & 0x8000 || (GetAsyncKeyState(VK_ESCAPE) & 0x8000))) // Выход из цикла на ESC, ENTER
    {
        gotoxy(0,choice);
        std::cout << ">";
        Sleep(100);
        gotoxy(0,choice);
        std::cout << " ";
        
        if ((GetAsyncKeyState(VK_UP) & 0x8000) || (GetAsyncKeyState(VK_DOWN) & 0x8000)) 
            choice = !choice;
    }
    return choice;                      
}

void menu()
{
    std::cout << "  PLAY" << "\n";
    std::cout << "  EXIT";
    if (choice_user())
        exit(0);
    Sleep(100);
    system("cls");
    std::cout << "  ONE PLAYER" << "\n";
    std::cout << "  TWO PLAYERS";
    choice_user();
    LoadMap();
}

void LoadMap()
{
    CurrentMap +=1;
    Level = Maps[CurrentMap]; //Подгружает из Maps.h нужный уровень
    for (int i = 0; i < std::size(Level); i++) //Печатает уровень в консоль
        std::cout << Level[i] << "\n";
}