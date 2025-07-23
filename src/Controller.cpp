#include <windows.h>
#include "Controller.h"
#include <iostream>
#include "Maps.h"
#include "Game.h"
#include <random>

unsigned int score = 0;
unsigned int points = 0;
unsigned int frameCount = 0;
unsigned int startHunt = 0;
unsigned int catched = 0;

bool scared = false;

std::vector<char> iconsGhost = {'R', 'P', 'B', 'Y'}; 

int offsetX = 0;
int offsetY = 0;

const int GAME_TICK = 10;       // минимальная задержка игры
int PLAYER_TICK_BASE = 10;      // задержка игрока
int GHOST_TICK_BASE = 30;       // базовая задержка призраков
int RED_ACCEL_STEP = 180;       // каждые 20 точек уменьшаем задержку
int RED_ACCEL = 10;             // насколько уменьшается задержка
int GHOST_TICK_MIN = 10;        // минимальная задержка призраков (макс. скорость)
int TIME_HUNT = 80000;            // время охоты игрока
int GHOST_TICK_HUNT = 40;       // задержка призрака при охоте игрока
const int RECALL = 1;           // задержка призрака при его смерти


/// Все что в блоке выше без const надо добавить в параметры карты

int lastTickSpeed = GHOST_TICK_BASE; // задержка до возращения на базу

int ScaredDir(Dir reverse, std::vector<bool> turns)
{
    static std::mt19937 engine{ std::random_device{}()};
    std::uniform_int_distribution<int> random(0,3);
    while(true)
    {    
        int i = random(engine);
        if (turns[i])
        {
            return i;
        }
    }
}


long GetConsoleWidth() // Получаем ширину консоли
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

long GetConsoleHeight() // Получаем высоту консоли
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

bool DrawScore(int score_row, unsigned int score1, unsigned int score2, int players)
{
    GoToxy(0, score_row);
    if (players == 1)
        std::cout << "Score: " << score1 << "    " << "Frame: " << frameCount << "    ";
    else
        std::cout << "P1: " << score1 << "   P2: " << score2;

    if (points == 436) // вроде 436 точек на карте
    {
        system("cls");
        GoToxy(50, Level.size() / 2);
        std::cout << "CONGRATULATIONS!";
        Sleep(5000);
        return true;
    }
    return false;
}

void CollectPoint(Player &p, std::vector<std::string> &level)
{
    if (level[p.y][p.x] == '.')
    {
        level[p.y][p.x] = ' ';
        ++p.score;
        points++;
    }
}

void CollectBuster(Player &p, std::vector<std::string> &level)
{
    if (level[p.y][p.x] == '%')
    {
        level[p.y][p.x] = ' ';
        p.score += 5;
        scared = true;
        startHunt = frameCount; 
    }
}

void GoToxy(int x, int y)
{
    COORD coord = {(short)(x + offsetX), (short)(y + offsetY)};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

bool IsWall(int x, int y)
{
    return Level[y][x] == '#';
}

int CountTurn(std::vector<bool> availableTurns)
{
    int count = 0;
    for (bool turn: availableTurns)
    {
        if (turn)
            count++;
    }
    return count;
}

// Перенесли логику движения одного призрака в отдельную функцию
void MoveOneGhost(std::vector<Ghost> &ghosts, size_t idx, Player &p1, Player &p2, int players, bool &p1Alive, bool &p2Alive)    
{
    Ghost &g = ghosts[idx];
    if (!IsWall(g.x, g.y))
    {
        GoToxy(g.x, g.y);
        std::cout << Level[g.y][g.x];
    }

    const Player *target = &p1;
    int gx = g.x;
    int gy = g.y;
    if (players == 2)
    {
        int d1 = abs(gx - p1.x) + abs(gy - p1.y);
        int d2 = abs(gx - p2.x) + abs(gy - p2.y);
        target = (d2 < d1 ? &p2 : &p1);
    }
    int tx = target->x;
    int ty = target->y;
    switch (g.icon)
    {
    case 'R': //Red, просто идёт за ближайшим игроком
    {
        tx = target->x;
        ty = target->y;
        break;
    }
    case 'P': //Pink, пытается обогнать игрока, идёт вперёд на 4 клетки по направлению движения игрока
    {
        tx += 4 * target->dx;
        ty += 4 * target->dy;
        break;
    }
    case 'B': //Blue, ещё хитрее, идёт на опрежение игрока, но от Blue
    {
        //Берёт точку на 2 клетки впереди игрока
        int projX = target->x + 2 * target->dx;
        int projY = target->y + 2 * target->dy;
        //Считает вектор от B (Blue) до этой точки и удваивает его
        int vx = projX - ghosts[0].x;
        int vy = projY - ghosts[0].y;
        tx = ghosts[0].x + 2 * vx;
        ty = ghosts[0].y + 2 * vy;
        break;
    }
    case 'Y':  //Yellow, ссыкло, если игрок далеко(>8 клеток) - преследует, иначе убегает
    { 
        int dist = abs(gx - target->x) + abs(gy - target->y);
        if (dist > 8)
        {
            tx = target->x;
            ty = target->y;
        }
        else
        {
            tx = 1;
            ty = Level.size() - 2; //угол карты
        }
        break;
    }
    case '0':
    {
        tx = 15;
        ty = 10;
        break;
    }
    }
    //Направления
    std::vector<Dir> directions = {{0, -1},
                                {0, 1},
                                {-1, 0},
                                {1, 0}};
    Dir reverse = {-g.dx, -g.dy};
    int bestDist = 10000;
    Dir bestDir = {-1, 0};
    int dist = bestDist;
    std::vector<bool> availableTurns = {true,true,true,true}; // Вверх, вниз, влево, вправо
    for (int i = 0; i < 4; i++)
    {
        Dir d = directions[i];
        if (d.dx == reverse.dx && d.dy == reverse.dy)
        {
            availableTurns[i] = false;
            continue;
        }
        int nx = gx + d.dx;
        int ny = gy + d.dy;
        if (IsWall(nx, ny))
        {
            availableTurns[i] = false;
            continue;
        }
        dist = abs(nx - tx) + abs(ny - ty);
        if (dist < bestDist)
        {
            bestDist = dist;
            bestDir = d;
        }
    }
    bool isFork = CountTurn(availableTurns) > 1;
    if (CountTurn(availableTurns) == 0)
    {
        bestDir = reverse;
    }
    if ((startHunt <= frameCount && frameCount <= (startHunt + GHOST_TICK_BASE)))
    {
        g.dx = -g.dx;
        g.dy = -g.dy;
    }
    else if (g.recalling)
    {
        p1.icon = '2'; /// надо решить проблему с возращением
        g.dx = bestDir.dx;
        g.dy = bestDir.dy;
    }
    else if (scared && isFork && !g.recalling)
    {
        Dir tempDir = directions[ScaredDir(reverse, availableTurns)];
        g.dx = tempDir.dx;
        g.dy = tempDir.dy;
    }
    else
    {
        g.dx = bestDir.dx;
        g.dy = bestDir.dy;
    }
    g.x += g.dx;
    g.y += g.dy;    

    if (g.x == 15 && g.y == 10)
    {
        g.recalling = false;
        g.icon = iconsGhost[idx];
    }


    if (((g.x == p1.x && g.y == p1.y) || (g.x == (p1.x + p1.dx) && g.y == (p1.y + p1.dy))) && p1Alive) //Поймали первого игрока
    {
        if (!scared)
        {
            p1Alive = false;
            GoToxy(50, Level.size() / 2);
            std::cout << "PLAYER 1 CAUGHT!";
            Sleep(1000);
        }
        else
        {
            catched++;
            p1.score += 40 * (catched + 1);
            g.recalling = true;
            g.icon = '0';
            g.tickPerMove = GHOST_TICK_MIN;
        }
    }
    if (players == 2 && ((g.x == p2.x && g.y == p2.y) || (g.x == (p2.x + p2.dx) && g.y == (p2.y + p2.dy))) && p2Alive) //Поймали второго игрока
    {
        if (!scared)
        {
            p2Alive = false;
            GoToxy(50, Level.size() / 2 + 1);
            std::cout << "PLAYER 2 CAUGHT!";
            Sleep(1000);
        }
        else
        {
            catched++;
            p2.score += 40 * (catched + 1);
            g.recalling = true;
            g.icon = '0';
            g.tickPerMove = GHOST_TICK_MIN;
        }
    }
    if (!p1Alive && (!p2Alive || players == 1)) //Всех игроков поймали
    { 
        system("cls");
        GoToxy(50, Level.size() / 2);
        std::cout << "GAME OVER!!!";
        Sleep(5000);
    }

    GoToxy(g.x, g.y);
    std::cout << g.icon;
}

bool Movement(int players, int &x1, int &y1, int &x2, int &y2)
{
    Player p1 = {x1, y1, 1, 0, '@'};
    Player p2 = {players == 2 ? x2 : 0,
                 players == 2 ? y2 : 0,
                 -1, 0, 'X'};
    bool p1Alive = true, p2Alive = (players == 2);


    std::vector<Ghost> ghosts = {{15, 11, 0, 0, 'R', false, GHOST_TICK_BASE, 0}};  //Red

    int score_row = Level.size(); //строка, в которой пишем очки

    while (!(GetAsyncKeyState(VK_ESCAPE) & 0x8000))
    {
        ++frameCount;
        
        if((frameCount - startHunt) == TIME_HUNT)
        {
            scared = false;
            catched = 0;
            for(int i = 0; i < ghosts.size(); i++)
            {
                ghosts[i].icon = iconsGhost[i];
            }
            ghosts[0].tickPerMove = lastTickSpeed;
        }
        if (scared)
        {
            for (int i = 0; i < ghosts.size(); i++)
            {
                if (!ghosts[i].recalling)
                    ghosts[i].icon = 'S';
            }
        }
        if((frameCount == 200 || points == 12) && ghosts.size() < 2)
            ghosts.push_back({17, 11, 0, 0, 'P', false, GHOST_TICK_BASE, 1});   //Pink    
        if((frameCount == 500 || points == 29) && ghosts.size() < 3)
            ghosts.push_back({19, 11, 0, 0, 'B', false, GHOST_TICK_BASE, 2});   //Blue
        if((frameCount == 1000 || points == 55) && ghosts.size() < 4)
            ghosts.push_back({21, 11, 0, 0, 'Y', false, GHOST_TICK_BASE, 3});   //Yellow
        

        if (frameCount % PLAYER_TICK_BASE == 0)
        {
            if (p1Alive && !IsWall(p1.x, p1.y))
            {
                GoToxy(p1.x, p1.y);
                std::cout << ' ';
            }
            if (players == 2 && p2Alive && !IsWall(p2.x, p2.y))
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

            // Следующие клетки 1 и 2 игрока
            int nx1 = p1.x + p1.dx, ny1 = p1.y + p1.dy;
            int nx2 = p2.x + p2.dx, ny2 = p2.y + p2.dy;

            // Проверка, могут ли попасть в следующие клетки игроки
            bool p1CanMove = !IsWall(nx1, ny1);
            bool p2CanMove = players == 2 && !IsWall(nx2, ny2);

            // Будущие координаты игроков с учётом стенок
            int tx1 = (p1CanMove ? nx1 : p1.x);
            int ty1 = (p1CanMove ? ny1 : p1.y);
            int tx2 = (p2CanMove ? nx2 : p2.x);
            int ty2 = (p2CanMove ? ny2 : p2.y);

            // Проверка на совпадание с 1 будущей клеткой
            bool sameCell = (tx1 == tx2 && ty1 == ty2);

            // Проверка на столкновение, чтобы упёрлись друг в друга а не прошли сквозь
            bool swapCell = (tx1 == p2.x && ty1 == p2.y && tx2 == p1.x && ty2 == p1.y);

            if (!sameCell && !swapCell)
            {
                p1.x = tx1; p1.y = ty1;
                if (players == 2)
                {
                    p2.x = tx2;
                    p2.y = ty2;
                }
            }

            if (p1Alive)
            {
                CollectBuster(p1,Level);
                CollectPoint(p1, Level);
                GoToxy(p1.x, p1.y);
                std::cout << p1.icon;
            }

            if (players == 2 && p2Alive)
            {
                CollectBuster(p1,Level);
                CollectPoint(p2, Level);
                GoToxy(p2.x, p2.y);
                std::cout << p2.icon;
            }
        }
        if (DrawScore(score_row, p1.score, p2.score, players))
            return true;


        for (size_t i = 0; i < ghosts.size(); ++i)
        {
            if (frameCount % ((ghosts[i].tickPerMove * (int)(!scared) + GHOST_TICK_HUNT * (int)(scared)) * !ghosts[i].recalling + ghosts[i].tickPerMove * ghosts[i].recalling) == 0)
            {
                if (ghosts[i].icon == 'R')
                {
                    unsigned int total = p1.score + (players == 2 ? p2.score : 0);
                    int accel = (total / RED_ACCEL_STEP) * RED_ACCEL;
                    ghosts[i].tickPerMove = std::max(GHOST_TICK_BASE - accel, GHOST_TICK_MIN);
                    lastTickSpeed = ghosts[i].tickPerMove;
                }
                MoveOneGhost(ghosts, i, p1, p2, players, p1Alive, p2Alive);
            }
        }

        if (GetAsyncKeyState('P') & 0x0001)
        {
            while (!(GetAsyncKeyState('P') & 0x0001))
            {
            }
        }
        Sleep(GAME_TICK);
    }
    return false;
}



/// TODO нужно сделать корректную ловлю игрока есть моменты когда игрок и призрак проскакивают сквозь себя  ✓
/// добавить функционал бустера
/// \> испуг призраков                      ✓
/// |> разворот на 180* в начале охоты      ✓
/// |> возможность съесть призрака          ✓
/// |> очки за призраков                    ✓ 
/// |> возвращение призраков                X


/// Реализовать ворота или сделать выпуск призраков более предсказуесмым X
