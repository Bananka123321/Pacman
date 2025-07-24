#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include <cstdlib>
#include "src/Game.h"
#include "src/Controller.h"
#include "src/Maps.h"


int main()
{
    system("mode con: cols=150 lines=40");//Pазмер консоли
    while (true)
    {
        system("cls");
        offsetX = GetConsoleWidth() / 2;
        offsetY = GetConsoleHeight() / 2;
        int players = Menu();//Выбор количества игроков
        CurrentMapIndex = -1;//Сброс на первый уровень

        while (true) //Цикл уровней
        {
            CurrentMapIndex++;
            if (CurrentMapIndex >= AllMaps.size()) {
                system("cls");
                std::cout << "!!!!!!!!CONGRATULATIONS!!!!!!!!\n\n" << "YOU COMPLETE THIS GAME\n\n";
                while(!(GetAsyncKeyState(VK_RETURN) & 0x8000))
                {

                }
                break; //Возвращаемся в меню
            }
            
            system("cls");
            
            LoadMap();

            DrawFruitDescriptions();
            
            int x1 = CurrentMap.HOME_PLAYER_1[0], y1 = CurrentMap.HOME_PLAYER_1[1];
            int x2 = CurrentMap.HOME_PLAYER_2[0], y2 = CurrentMap.HOME_PLAYER_2[1];

            if (!Movement(players, x1, y1, x2, y2)) {
                break; //Возвращаемся в главное меню
            }

            else {
                system("cls");
                GoToxy(0, CurrentMap.layout.size() / 2);
                std::cout << "LEVEL COMPLETE!";
                Sleep(2000);
            }
        }
    }
}
