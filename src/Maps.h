#pragma once
#include <vector>
#include <string>

struct MapData { //Структура карт, содержащая переменные для динамического геймплея
    std::vector<std::string> layout;
    int points;

    int PLAYER_TICK_BASE;      // задержка игрока
    int GHOST_TICK_BASE;       // базовая задержка призраков
    int RED_ACCEL_STEP;        // каждые 20 точек уменьшаем задержку
    int RED_ACCEL;             // насколько уменьшается задержка
    int GHOST_TICK_MIN;        // минимальная задержка призраков (макс. скорость)
    int TIME_HUNT;             // время охоты игрока
    int GHOST_TICK_HUNT;       // задержка призрака при охоте игрока
    std::vector<int> HOME_PLAYER_1;
    std::vector<int> HOME_PLAYER_2;
    std::vector<int> HOME_RED;
    std::vector<int> HOME_PINK;
    std::vector<int> HOME_BLUE;
    std::vector<int> HOME_YELLOW;
};

extern std::vector<MapData> AllMaps;
extern MapData CurrentMap;
extern int CurrentMapIndex;//Счётчик пройденных уровней
