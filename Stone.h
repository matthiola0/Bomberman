#ifndef STONE_H_INCLUDE
#define STONE_H_INCLUDE

#include <stdio.h>
#include "Object.h"
#include "Circle.h"
#include "global.h"

class Stone : public Object {
public:
    Stone(int, int);
    virtual ~Stone();

    void Draw();
    void Load();
    bool is_on_map() {return on_map;}
private:
    // images
    ALLEGRO_BITMAP* img = NULL;
    bool on_map;
};

#endif // STONE_H_INCLUDE