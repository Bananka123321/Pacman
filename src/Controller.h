#pragma once
#include <vector>
#include <string>

extern unsigned int score;

void GoToxy(int x, int y);
bool IsWall(int x, int y);//Проверяет есть ли стена
void Movement(int &x, int &y);