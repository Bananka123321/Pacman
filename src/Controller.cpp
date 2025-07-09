#include <windows.h>
#include "Controller.h"
#include <iostream>
#include "Maps.h"
#include "Game.h"

unsigned int score = 0;

void GoToxy(int x, int y)
{
    COORD coord = {(short)x, (short)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

bool IsWall(int x, int y)
{
    return Level[y][x] == '#';
}

bool IsPoint(int x, int y)
{
    if (Level[y][x] == '.')
    {
        Level[y][x] = ' ';
        return true;
    }
    return false;
}

void Movement(int players, int &x1, int &y1, int &x2, int &y2)
{
    Player p1 = {x1, y1, 1, 0, '@'};
    Player p2 = {x2, y2, 0, 0, 'X'};

    while (!(GetAsyncKeyState(VK_ESCAPE) & 0x8000))
    {
        // Управление игроками
        // Игрок 1 - стрелки
        if (GetAsyncKeyState(VK_UP) & 0x8000)    { p1.dx = 0; p1.dy = -1; }
        if (GetAsyncKeyState(VK_DOWN) & 0x8000)  { p1.dx = 0; p1.dy = 1; }
        if (GetAsyncKeyState(VK_LEFT) & 0x8000)  { p1.dx = -1; p1.dy = 0; }
        if (GetAsyncKeyState(VK_RIGHT) & 0x8000) { p1.dx = 1; p1.dy = 0; }

        // Игрок 2 - WASD
        if (players == 2) {
            if (GetAsyncKeyState('W') & 0x8000) { p2.dx = 0; p2.dy = -1; }
            if (GetAsyncKeyState('S') & 0x8000) { p2.dx = 0; p2.dy = 1; }
            if (GetAsyncKeyState('A') & 0x8000) { p2.dx = -1; p2.dy = 0; }
            if (GetAsyncKeyState('D') & 0x8000) { p2.dx = 1; p2.dy = 0; }
        }

        // Стереть старые позиции
        GoToxy(p1.x, p1.y); std::cout << " ";
        if (players == 2) {
            GoToxy(p2.x, p2.y); std::cout << " ";
        }

        // Переместить игроков, если нет стены
        if (!IsWall(p1.x + p1.dx, p1.y + p1.dy)) {
            p1.x += p1.dx;
            p1.y += p1.dy;
        }

        if (players == 2 && !IsWall(p2.x + p2.dx, p2.y + p2.dy)) {
            p2.x += p2.dx;
            p2.y += p2.dy;
        }

        // Столкновение игроков
        if (players == 2 && p1.x == p2.x && p1.y == p2.y) {
            GoToxy(0, Level.size() + 2);
            std::cout << "Игроки столкнулись!";
            break;
        }

        // Нарисовать новых
        GoToxy(p1.x, p1.y); std::cout << p1.icon;
        if (players == 2) {
            GoToxy(p2.x, p2.y); std::cout << p2.icon;
        }

        Sleep(150);
    }
}