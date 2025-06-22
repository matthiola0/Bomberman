#ifndef BOMB_H_INCLUDE
#define BOMB_H_INCLUDE

#include <stdio.h>
#include "Object.h"
#include "Circle.h"
#include "global.h"

class Bomb : public Object {
public:
    Bomb(int, int, char*);
    virtual ~Bomb();

    void Draw();
    void Load(char*);
    void DrawFire();
    void DrawFire(bool, bool, bool, bool);

    int get_character() { return character; }

    void change_counter();
    int get_counter();

private:
    // images
    ALLEGRO_BITMAP* img = NULL;
    ALLEGRO_BITMAP* img_left = NULL;
    ALLEGRO_BITMAP* img_right = NULL;
    ALLEGRO_BITMAP* img_up = NULL;
    ALLEGRO_BITMAP* img_down = NULL;
    int character;
    int counter;
};

#endif // BOMB_H_INCLUDE