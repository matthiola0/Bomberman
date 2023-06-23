#include "Bomb.h"

Bomb::Bomb(int x, int y, char* buffer) 
{
    circle = new Circle;
    circle->x = (int)((x+grid_width/2)/grid_width) * grid_width;
    circle->y = (int)((y+grid_height/2)/grid_height) * grid_height;
    circle->r = grid_width/2;
    counter = 0;

    Load(buffer);
}

Bomb::~Bomb()
{
    al_destroy_bitmap(img);
    al_destroy_bitmap(img_left);
    al_destroy_bitmap(img_right);
    al_destroy_bitmap(img_up);
    al_destroy_bitmap(img_down);

    delete circle;
}

void
Bomb::Load(char* name)
{
    char buffer[50];

    sprintf(buffer, "./Asset/%s_bomb.png", name);

    if (!strcmp(name, "bocchi")) 
        character = BOCCHI;
    else if (!strcmp(name, "ijichi"))
        character = IJICHI;
    else if (!strcmp(name, "yamada"))
        character = YAMADA; 
    else if (!strcmp(name, "kita"))
        character = KITA;


    img = al_load_bitmap(buffer);
    img_left = al_load_bitmap("./Asset/fire_left.png");
    img_right = al_load_bitmap("./Asset/fire_right.png");
    img_up = al_load_bitmap("./Asset/fire_up.png");
    img_down = al_load_bitmap("./Asset/fire_down.png");
}

void
Bomb::Draw()
{
    al_draw_bitmap(img, circle->x - circle->r, circle->y - circle->r, 0);
    // printf("PASS\n");
}

void
Bomb::DrawFire()
{
    al_draw_bitmap(img_left, circle->x-grid_width - circle->r, circle->y - circle->r, 0);
    al_draw_bitmap(img_right, circle->x+grid_width - circle->r, circle->y - circle->r, 0);
    al_draw_bitmap(img_down, circle->x - circle->r, circle->y-grid_height - circle->r, 0);
    al_draw_bitmap(img_up, circle->x - circle->r, circle->y+grid_height - circle->r, 0);
}

void
Bomb::DrawFire(bool left, bool right, bool up, bool down)
{
    if(left)
        al_draw_bitmap(img_left, circle->x-grid_width - circle->r, circle->y - circle->r, 0);
    if(right)
        al_draw_bitmap(img_right, circle->x+grid_width - circle->r, circle->y - circle->r, 0);
    if(up)
        al_draw_bitmap(img_down, circle->x - circle->r, circle->y-grid_height - circle->r, 0);
    if(down)
        al_draw_bitmap(img_up, circle->x - circle->r, circle->y+grid_height - circle->r, 0);
}

void 
Bomb::change_counter() 
{
    counter++;
}

int 
Bomb::get_counter() 
{
    return counter;
}