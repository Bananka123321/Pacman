#pragma once
#include <vector>
#include <string>

extern unsigned int score;

void GoToxy(int x, int y);
bool IsWall(int x, int y);//Проверяет есть ли стена
void Movement(int players, int &x1, int &y1, int &x2, int &y2);

struct Player {
    int x, y;
    int dx = 0, dy = 0; // направление
    char icon;
};