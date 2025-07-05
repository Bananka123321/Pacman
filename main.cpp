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
    menu();

    int x = 4, y = 1;
    Movement(x,y);
}
