#ifndef LEVEL_H_INCLUDED
#define LEVEL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include "global.h"

typedef struct Node {
    // int h, g, f;
    bool roadPoint = false;
    bool stone = false;
    bool have_color = false;
    int character = 0;
    bool bombing = false;  //fire

    bool speed_tool = false;
}Node;

class LEVEL {
public:
    LEVEL(const int);
    ~LEVEL();

    void Reset(int);
    
    void setLevel(const int);
    bool isRoad(int index) { 
        if (index < 0 || index >= NumOfGrid) return false;
        return levelMap[index].roadPoint; 
    }
    bool isStone(int index) { 
        if (index < 0 || index >= NumOfGrid) return false;
        return levelMap[index].stone; 
    }
    bool haveColor(int index) { 
        if (index < 0 || index >= NumOfGrid) return false;
        return levelMap[index].have_color; 
    }
    bool is_bombing(int index) { 
        if (index < 0 || index >= NumOfGrid) return false;
        return levelMap[index].bombing; 
    }
    void bomb_yes(int index) { 
        if (index >= 0 && index < NumOfGrid) levelMap[index].bombing = true; 
    } 
    void bomb_not(int index) { 
        if (index >= 0 && index < NumOfGrid) levelMap[index].bombing = false; 
    }
    int get_character(int index) { 
        if (index < 0 || index >= NumOfGrid) return 0;
        return levelMap[index].character; 
    }
    void change_character(int index, int ch) {
        if (index >= 0 && index < NumOfGrid) {
            levelMap[index].have_color = true;
            levelMap[index].character = ch;
        }
    }

    void stone_bomb(int index) {
        if (index >= 0 && index < NumOfGrid) {
            levelMap[index].roadPoint = true;
            levelMap[index].stone = false;
        }
    }

    bool have_speed(int index) {
        if (index < 0 || index >= NumOfGrid) return false;
        return levelMap[index].speed_tool;
    }
    void speed_emerge(int index) {
        if (index >= 0 && index < NumOfGrid) levelMap[index].speed_tool = true;
    }
    void speed_disappear(int index) {
        if (index >= 0 && index < NumOfGrid) levelMap[index].speed_tool = false;
    }

    void clear_color(int);

    // int getMonsterSpeed() { return MonsterSpeed; }
    int getLevel() { return level; }
    //void search_destination();
    // std::vector<int> ReturnPath() { return road_grid; }

    // int MonsterNum[Num_MonsterType];

private:
    Node levelMap[NumOfGrid];
    // std::vector<int> road_grid;
    // current level number
    int level = 1;
    // maximum number of monster of a level
    // int Monster_MAX = 50;
    // frequency of producing  monster
    // int MonsterSpeed = 80;
};


#endif // LEVEL_H_INCLUDED
