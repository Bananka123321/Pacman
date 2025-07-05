#pragma once
#include <vector>
#include <string>

void gotoxy(int x, int y);
bool IsWall(int x, int y);//Проверяет есть ли стена
bool Movement(int &x, int &y);