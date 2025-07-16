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

struct Ghost {
    int x, y;
    int dx, dy;
    char icon;
    int type;// 0 - Red, 1 - Pink, 2 - Blue, 3 - Yellow
};

struct Dir { int dx, dy; };

void MoveGhosts(std::vector<Ghost>& ghosts, const Player& p1, const Player& p2, int players);

void CollectPoint(Player& p, const std::vector<std::string>& level);