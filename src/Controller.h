#pragma once
#include <vector>
#include <string>

extern unsigned int score;

void GoToxy(int x, int y);
bool IsWall(int x, int y);//Проверяет есть ли стена
void Movement(int players, int &x1, int &y1, int &x2, int &y2);

void DrawScore(int score_column, unsigned int score1, unsigned int score2, int players);//Вывод счёта игрока/игроков

struct Player {
    int x, y;
    int dx, dy;
    char icon;
    unsigned int score = 0;//Счёт каждого игрока
};

void CollectPoint(Player& p, const std::vector<std::string>& level);