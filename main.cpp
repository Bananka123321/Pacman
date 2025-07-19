#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include <cstdlib>
#include "src/Game.h"
#include "src/Controller.h"


int main()
{
    while (true)
    {
        system("cls");
        int players = Menu();//Выбор количества игроков
        CurrentMap = -1;//Сброс на первый уровень

        while (true) //Цикл уровней
        {
            CurrentMap++;
            if (CurrentMap >= 10) {
                break; //Возвращаемся в меню
            }

            LoadMap();

            int x1 = 4, y1 = 1;
            int x2 = 4, y2 = 1;

            if (!Movement(players, x1, y1, x2, y2)) {
                break; //Возвращаемся в главное меню
            }

            else {
                system("cls");
                GoToxy(50, Level.size() / 2);
                std::cout << "LEVEL COMPLETE!";
                Sleep(2000);
            }
        }
    }
}
