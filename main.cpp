#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include <cstdlib>
#include "src/Game.h"
#include "src/Controller.h"



std::vector<std::string> level = {
    "####################",
    "#                  #",
    "#  ####  ##  ####  #",
    "#                  #",
    "####################"};

int main()
{
    menu();
    system("cls");
    for (int i = 0; i < std::size(level); i++)
        std::cout << level[i] << "\n";
    int x = 10, y = 10;

    while(!(GetAsyncKeyState(VK_ESCAPE) & 0x8000))
    {
        gotoxy(x, y);
        std::cout << "@";

        Sleep(100);

        gotoxy(x, y);
        std::cout << " ";

        if (GetAsyncKeyState(VK_UP) & 0x8000)
            y--;
        if (GetAsyncKeyState(VK_DOWN) & 0x8000)
            y++;
        if (GetAsyncKeyState(VK_LEFT) & 0x8000)
            x--;
        if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
            x++;
    }
}
