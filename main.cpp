#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include <cstdlib>
#include "src/Game.h"
#include "src/Controller.h"


int main()
{
    system("cls");
    int players = Menu();
    int x1 = 4, y1 = 1;
    int x2 = 4, y2 = 1;

    Movement(players, x1, y1, x2, y2);
}
