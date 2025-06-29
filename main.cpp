#include <iostream>
#include <windows.h>
#include <vector>
#include <string>


void gotoxy(int x, int y) {
    COORD coord = { (short)x, (short)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

std::vector<std::string> level = {
    "####################",
    "#                  #",
    "#  ####  ##  ####  #",
    "#                  #",
    "####################"
};

int main() {
    int x = 10, y = 10;
 
    while (true) {
        gotoxy(x, y);
        std::cout << "@";   

        Sleep(100);    

        gotoxy(x, y);
        std::cout << " ";

        if (GetAsyncKeyState(VK_UP) & 0x8000) y--;
        if (GetAsyncKeyState(VK_DOWN) & 0x8000) y++;
        if (GetAsyncKeyState(VK_LEFT) & 0x8000) x--;
        if (GetAsyncKeyState(VK_RIGHT) & 0x8000) x++;
    }
}
