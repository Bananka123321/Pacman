#pragma once
#include <vector>
#include <string>

extern unsigned int score;
extern unsigned int frameCount;
extern unsigned int finishScore;


void GoToxy(int x, int y);
bool IsWall(int x, int y);//Проверяет есть ли стена
bool Movement(int players, int &x1, int &y1, int &x2, int &y2);

bool DrawScore(int score_column, unsigned int score1, unsigned int score2, int lives1, int lives2, int players);//Вывод счёта игрока/игроков

struct Player {
    int x, y;
    int dx, dy;
    char icon;
    unsigned int score = 0;//Счёт каждого игрока
    int lives = 2;//Количество жизней игроков, на 1 меньше, так как проверка на количество жизней идёт во время поимки а не после уменьшения количества жизней
};

struct Ghost {
    int x, y;
    int dx, dy;
    char icon;
    bool recalling;
    bool beEaten;
    int tickPerMove;
    int type;// 0 - Red, 1 - Pink, 2 - Blue, 3 - Yellow
};

struct Dir { int dx, dy; };

bool MoveGhosts(std::vector<Ghost>& ghosts, const Player& p1, const Player& p2, int players, bool& p1Alive, bool& p2Alive);

void CollectPoint(Player& p, const std::vector<std::string>& level);

long GetConsoleWidth();
long GetConsoleHeight();

extern int offsetX;
extern int offsetY;

void Respawn(std::vector<Ghost>& ghosts);
void Respawn(Player& Player, int players);//Респавн игрока в начале карты, если есть жизни