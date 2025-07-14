#include <windows.h>
#include "Controller.h"
#include <iostream>
#include "Maps.h"
#include "Game.h"

unsigned int score = 0;

void DrawScore(int score_row, unsigned int score1, unsigned int score2, int players)
{
    GoToxy(0, score_row);
    if (players == 1)
        std::cout << "Score: " << score1 << "    ";
    else
        std::cout << "P1: " << score1 << "   P2: " << score2;
}

void CollectPoint(Player& p, std::vector<std::string>& level)
{
    if (level[p.y][p.x] == '.'){
        level[p.y][p.x] = ' ';
        ++p.score;
    }
}

void GoToxy(int x, int y)
{
    COORD coord = {(short)x, (short)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

bool IsWall(int x, int y)
{
    return Level[y][x] == '#';
}

void Movement(int players, int &x1, int &y1, int &x2, int &y2)
{
    Player p1 = {x1, y1, 1, 0, '@'};
    Player p2 = {players == 2 ? x2 : 0, players == 2 ? y2 : 0, -1, 0, 'X'};

    int score_row = Level.size();   //строка, в которой пишем очки

    while (!(GetAsyncKeyState(VK_ESCAPE) & 0x8000))
    {

        if (!IsWall(p1.x, p1.y))
        {
            GoToxy(p1.x, p1.y); 
            std::cout << ' ';
        }
        if (players == 2 && !IsWall(p2.x, p2.y))
        {
            GoToxy(p2.x, p2.y); 
            std::cout << ' ';
        }


        if (GetAsyncKeyState(VK_UP)    & 0x8000 && !IsWall(p1.x, p1.y - 1)) { p1.dx = 0;  p1.dy = -1; }
        if (GetAsyncKeyState(VK_DOWN)  & 0x8000 && !IsWall(p1.x, p1.y + 1)) { p1.dx = 0;  p1.dy =  1; }
        if (GetAsyncKeyState(VK_LEFT)  & 0x8000 && !IsWall(p1.x - 1, p1.y)) { p1.dx = -1; p1.dy =  0; }
        if (GetAsyncKeyState(VK_RIGHT) & 0x8000 && !IsWall(p1.x + 1, p1.y)) { p1.dx = 1;  p1.dy =  0; }

        if (players == 2) {
            if (GetAsyncKeyState('W') & 0x8000 && !IsWall(p2.x, p2.y - 1)) { p2.dx = 0;  p2.dy = -1; }
            if (GetAsyncKeyState('S') & 0x8000 && !IsWall(p2.x, p2.y + 1)) { p2.dx = 0;  p2.dy =  1; }
            if (GetAsyncKeyState('A') & 0x8000 && !IsWall(p2.x - 1, p2.y)) { p2.dx = -1; p2.dy =  0; }
            if (GetAsyncKeyState('D') & 0x8000 && !IsWall(p2.x + 1, p2.y)) { p2.dx = 1;  p2.dy =  0; }
        }



        //Следующие клетки 1 и 2 игрока
        int nx1 = p1.x + p1.dx, ny1 = p1.y + p1.dy;
        int nx2 = p2.x + p2.dx, ny2 = p2.y + p2.dy;

        //Проверка, могут ли попасть в следующие клетки игроки
        bool p1CanMove = !IsWall(nx1, ny1);
        bool p2CanMove = players == 2 && !IsWall(nx2, ny2);

        //Будущие координаты игроков с учётом стенок
        int tx1 = (p1CanMove ? nx1 : p1.x);
        int ty1 = (p1CanMove ? ny1 : p1.y);
        int tx2 = (p2CanMove ? nx2 : p2.x);
        int ty2 = (p2CanMove ? ny2 : p2.y);

        //Проверка на совпадание с 1 будущей клеткой
        bool sameCell = (tx1 == tx2 && ty1 == ty2);
        
        //Проверка на столкновение, чтобы упёрлись друг в друга а не прошли сквозь
        bool swapCell = (tx1 == p2.x && ty1 == p2.y && tx2 == p1.x && ty2 == p1.y);

        if (!sameCell && !swapCell) {
            p1.x = tx1; p1.y = ty1;
            if (players == 2) { p2.x = tx2; p2.y = ty2; }
        }

        CollectPoint(p1, Level);

        GoToxy(p1.x, p1.y); std::cout << p1.icon;
        if (players == 2) {
            CollectPoint(p2, Level);
            GoToxy(p2.x, p2.y); std::cout << p2.icon;
        }

        DrawScore(score_row, p1.score, p2.score, players);

        Sleep(150);

        if (GetAsyncKeyState('P') & 0x0001)
        {
            while(!(GetAsyncKeyState('P') & 0x0001))
            {}
        }
    }
}
