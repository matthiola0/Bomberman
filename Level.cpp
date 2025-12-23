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
    if (!file) {
        fprintf(stderr, "Error: Could not open level file %s\n", buffer);
        return;
    }

    this->level = level;
    
    for(int i = 0; i < NumOfGrid; i++)
    {
        this->levelMap[i].roadPoint = false;
        this->levelMap[i].stone = false;
        this->levelMap[i].have_color = false;
        this->levelMap[i].character = 0;
        this->levelMap[i].speed_tool = false;
    }

    int tmp_val;
    while(fscanf(file, "%s", buffer) != EOF) {
        tmp_val = atoi(buffer);
        bool is_stone = false;
        if (tmp_val > 1000) {
            tmp_val -= 1000;
            is_stone = true;
        }

        if (tmp_val >= 0 && tmp_val < NumOfGrid) {
            if (is_stone) {
                this->levelMap[tmp_val].stone = true;
                this->levelMap[tmp_val].roadPoint = false;
            } else {
                this->levelMap[tmp_val].stone = false;
                this->levelMap[tmp_val].roadPoint = true;
            }
            this->levelMap[tmp_val].have_color = false;
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