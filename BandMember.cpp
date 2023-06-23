#include "BandMember.h"

// const int axis_x[] = {-1, 1, 0, 0};
// const int axis_y[] = {0, 0, -1, 1};
// const char direction_name[][10] = {"LEFT", "RIGHT", "UP", "DOWN"};

// set counter frequency of drawing moving animation
// const int draw_frequency = 10;

BandMember::BandMember(int _character)
{
    // this->path = path;
    // this->step = 0;

    character = _character;

    // default direction is right
    direction = RIGHT;

    switch (character)
    {
        case BOCCHI:
            init_pos = 94;
            strncpy(class_name, BandMemberClass[BOCCHI], 20);
            Load();
            break;
        case IJICHI:
            init_pos = 102;
            strncpy(class_name, BandMemberClass[IJICHI], 20);
            Load();
            break;
        case YAMADA:
            init_pos = 169;
            strncpy(class_name, BandMemberClass[YAMADA], 20);
            Load();
            break;
        case KITA:
            init_pos = 177;
            strncpy(class_name, BandMemberClass[KITA], 20);
            Load();
            break;
    }
    
    speed = 1;
    speed_counter = 0;
    circle = new Circle;
    circle->x = (init_pos % 15) * grid_width;
    circle->y = (init_pos / 15) * grid_height;
    circle->r = member_width/2;

}

BandMember::~BandMember()
{
    
    al_destroy_bitmap(img);

    delete circle;
}

void 
BandMember::Reset() {
    direction = RIGHT;

    switch (character)
    {
        case BOCCHI:
            init_pos = 94;
            break;
        case IJICHI:
            init_pos = 102;
            break;
        case YAMADA:
            init_pos = 169;
            break;
        case KITA:
            init_pos = 177;
            break;
    }
    speed = 1;
    speed_counter = 0;
    circle->x = (init_pos % 15) * grid_width;
    circle->y = (init_pos / 15) * grid_height;
}
void
BandMember::Load()
{
    char buffer[50];

    sprintf(buffer, "./Asset/%s.png", class_name);

    img = al_load_bitmap(buffer);

}

void
BandMember::Draw()
{
    // int w, h;
    // int offset = 0;

    al_draw_bitmap(img, circle->x - circle->r, circle->y - circle->r, 0);

    ///if(!moveImg[offset + sprite_pos])
        //return;

    // get height and width of sprite bitmap
    //w = al_get_bitmap_width(moveImg[offset + sprite_pos]);
    //h = al_get_bitmap_height(moveImg[offset + sprite_pos]);


    // draw bitmap align grid edge
    //al_draw_bitmap(moveImg[offset + sprite_pos], circle->x - w/2, circle->y - (h - grid_height/2), 0);

    //al_draw_filled_circle(circle->x, circle->y, circle->r, al_map_rgba(196, 79, 79, 200));
}

bool
BandMember::Move()
{

    circle->x += speed * axis_x[direction];
    circle->y += speed * axis_y[direction];

    // if not reaching end point, return false
    return false;
}

void
BandMember::changeDir(int dir) {
    // direction = dir;
    switch(dir) {
        case 0:
            direction = LEFT;
            break;
        case 1:
            direction = RIGHT;
            break;
        case 2:
            direction = UP;
            break;
        case 3:
            direction = DOWN;
            break;
    }
}
