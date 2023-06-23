#include "Stone.h"

Stone::Stone(int x, int y) 
{
    circle = new Circle;
    circle->x = (int)(x/grid_width) * grid_width;
    circle->y = (int)(y/grid_height) * grid_height;
    circle->r = grid_width/2;

    on_map = true;

    Load();
}

Stone::~Stone()
{
    al_destroy_bitmap(img);

    delete circle;
}

void
Stone::Load()
{
    img = al_load_bitmap("./Asset/chair.png");
}
 
void
Stone::Draw()
{
    al_draw_bitmap(img, circle->x - circle->r, circle->y - circle->r, 0);
}
