#include <windows.h>
#include "Controller.h"
#include <iostream>
#include "Maps.h"
#include "Game.h"

void gotoxy(int x, int y)
{
    COORD coord = {(short)x, (short)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

bool IsWall(int x, int y)
{
    return Level[y][x] == '#';
}

bool Movement(int &x, int &y)
{
while(!(GetAsyncKeyState(VK_ESCAPE) & 0x8000))
    {
        gotoxy(x, y);
        std::cout << "@";

        Sleep(100);

        gotoxy(x, y);
        std::cout << " ";

        if (GetAsyncKeyState(VK_UP) & 0x8000 && !IsWall(x, y - 1))
            y--;
        if (GetAsyncKeyState(VK_DOWN) & 0x8000 && !IsWall(x, y + 1))
            y++;
        if (GetAsyncKeyState(VK_LEFT) & 0x8000 && !IsWall(x - 1, y))
            x--;
        if (GetAsyncKeyState(VK_RIGHT) & 0x8000 && !IsWall(x + 1, y))
            x++;
    }
}