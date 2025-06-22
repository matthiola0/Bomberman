#ifndef BANDMEMBER_H_INCLUDED
#define BANDMEMBER_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <vector>
#include "Object.h"
#include "Circle.h"
#include "global.h"
#include "Bomb.h"

const char direction_name[][10] = {"LEFT", "RIGHT", "UP", "DOWN"};


class BandMember: public Object {
public:
    BandMember(int);
    virtual ~BandMember();

    void Reset();
    // Draw image per frame
    // override virtual function "Object::Draw"
    void Draw();
    // Load bitmaps of animation image into container "img"
    bool Load();

    // void put_bomb();


    // Update monster position per frame
    // And detect if it reaches end point but not destroyed
    bool Move();

    void change_speed(int _speed) { speed = _speed; }

    // functions that return informations of monster
    int getDir() { return direction; }
    // int getWorth() { return worth; }
    int getScore() { return score; }

    void SpeedChange(int S) {
        speed_counter = 60 * 5; 
        speed = S; 
    }
    int getSCounter() { return speed_counter; }
    
    void changeDir(int);
    void change_counter() { 
        if (counter > 0)
            counter -= 1;
        if (speed_counter > 0)
            speed_counter -= 1;
    }

protected:
    // int direction_count[4];
    // int HealthPoint = 1;
    int speed = 1;
    // int worth = 10;
    int score = 0;
    char class_name[20];
    int character;

    int init_pos = 94;

    int direction;
private:
    // animation counter
    int speed_counter;
    int counter;
    // animation image of current direction
    int sprite_pos;

    // images
    ALLEGRO_BITMAP* img = NULL;
    // path on map
    // std::vector<int> path;

};


#endif // BANDMEMBER_H_INCLUDED
