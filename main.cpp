#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include <cstdlib>

void gotoxy(int x, int y)
{
    COORD coord = {(short)x, (short)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int choice_user()
{
    bool choice = 0; // Выбор пользователя 0 - PLAY, 1 - EXIT
    while(!(GetAsyncKeyState(VK_RETURN) & 0x8000 || (GetAsyncKeyState(VK_ESCAPE) & 0x8000))) // Выход из цикла на ESC, ENTER
    {
        gotoxy(0,choice);
        std::cout << ">";
        Sleep(100);
        gotoxy(0,choice);
        std::cout << " ";
        
        if ((GetAsyncKeyState(VK_UP) & 0x8000) || (GetAsyncKeyState(VK_DOWN) & 0x8000)) 
            choice = !choice;
    }
    return choice;                      
}

void menu()
{
    std::cout << "  PLAY" << "\n";
    std::cout << "  EXIT";
    choice_user();
}

std::vector<std::string> level = {
    "####################",
    "#                  #",
    "#  ####  ##  ####  #",
    "#                  #",
    "####################"};

int main()
{

    menu();
    system("cls");
    for (int i = 0; i < std::size(level); i++)
        std::cout << level[i] << "\n";
    int x = 10, y = 10;

    while(!(GetAsyncKeyState(VK_ESCAPE) & 0x8000))
    {
        gotoxy(x, y);
        std::cout << "@";

        Sleep(100);

        gotoxy(x, y);
        std::cout << " ";

        if (GetAsyncKeyState(VK_UP) & 0x8000)
            y--;
        if (GetAsyncKeyState(VK_DOWN) & 0x8000)
            y++;
        if (GetAsyncKeyState(VK_LEFT) & 0x8000)
            x--;
        if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
            x++;
    }
}
