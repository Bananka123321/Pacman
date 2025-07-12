#pragma once
#include <vector>
#include <string>

bool ChoiceUser();
int Menu();
void LoadMap();//Загрузка уровня

extern int CurrentMap;//Счётчик пройденных уровней
extern std::vector<std::string> Level; //Текущий уровень