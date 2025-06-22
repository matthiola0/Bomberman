#include "Level.h"

LEVEL::LEVEL(const int level) {
    char buffer[50];

    setLevel(level);
}

LEVEL::~LEVEL()
{
    for(int i=0;i < NumOfGrid;i++)
    {
        this->levelMap[i].roadPoint = false;
        this->levelMap[i].stone = false;
        this->levelMap[i].have_color = false;
        this->levelMap[i].character = 0;
        this->levelMap[i].speed_tool = false;
    }
}

void 
LEVEL::Reset(int _level)
{
    level = _level;
    setLevel(level);
}

void
LEVEL::setLevel(const int level)
{
    char buffer[50];
    FILE *file;

    sprintf(buffer, "LEVEL%d.txt", level);
    file = fopen(buffer, "r");

    this->level = level;
    // this->road_grid.clear();

    for(int i = 0; i < NumOfGrid; i++)
    {
        this->levelMap[i].roadPoint = false;
        this->levelMap[i].stone = false;
        this->levelMap[i].have_color = false;
        this->levelMap[i].character = 0;
        this->levelMap[i].speed_tool = false;
    }

    int tmp;
    while(fscanf(file, "%s", buffer) != EOF) {
        int tmp = atoi(buffer);
        if (tmp > 1000) {
            tmp -= 1000;
            this->levelMap[tmp].stone = true;
            this->levelMap[tmp].roadPoint = false;
            this->levelMap[tmp].have_color = false;
        }
        else {
            this->levelMap[tmp].stone = false;
            this->levelMap[tmp].roadPoint = true;
            this->levelMap[tmp].have_color = false;
        }
    }

    fclose(file);
}

void
LEVEL::clear_color(int _character) {
    for (int i = 0; i < NumOfGrid; i++) {
        if (levelMap[i].have_color == true) {
            if (levelMap[i].character == _character) 
                levelMap[i].have_color = false, levelMap[i].character = 0;
        }
    }
}