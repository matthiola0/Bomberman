#include "BandMember.h"
#include <stdio.h>

// const int axis_x[] = {-1, 1, 0, 0};
// const int axis_y[] = {0, 0, -1, 1};
// const char direction_name[][10] = {"LEFT", "RIGHT", "UP", "DOWN"};

BandMember::BandMember(int _character)
{
    img = NULL; 
    character = _character;
    direction = RIGHT;

    switch (character)
    {
        case BOCCHI:
            init_pos = 94;
            strncpy(class_name, BandMemberClass[BOCCHI], 20);
            break;
        case IJICHI:
            init_pos = 102;
            strncpy(class_name, BandMemberClass[IJICHI], 20);
            break;
        case YAMADA:
            init_pos = 169;
            strncpy(class_name, BandMemberClass[YAMADA], 20);
            break;
        case KITA:
            init_pos = 177;
            strncpy(class_name, BandMemberClass[KITA], 20);
            break;
    }

    if (!Load()) {
        fprintf(stderr, "Error: Could not load assets for character '%s'.\n", class_name);
    }
    
    speed = 1;
    speed_counter = 0;
    circle = new Circle;
    circle->r = member_width / 2;
    Reset(15); // Default to 15x15 layout
}

BandMember::~BandMember()
{
    if (img) {
        al_destroy_bitmap(img);
    }
    delete circle;
}

void 
BandMember::Reset(int lw) {
    direction = RIGHT;
    int col = 0, row = 0;
    if (lw == 25) {
        switch (character) {
            case BOCCHI: col = 1; row = 1; break;
            case IJICHI: col = 23; row = 1; break;
            case YAMADA: col = 1; row = 23; break;
            case KITA: col = 23; row = 23; break;
        }
    } else {
        switch (character) {
            case BOCCHI: col = 1; row = 1; break;
            case IJICHI: col = 13; row = 1; break;
            case YAMADA: col = 1; row = 13; break;
            case KITA: col = 13; row = 13; break;
        }
    }
    speed = 2;
    speed_counter = 0;
    circle->x = col * grid_width + grid_width/2;
    circle->y = row * grid_height + grid_height/2;
}

bool
BandMember::Load()
{
    char buffer[50];
    sprintf(buffer, "src/Asset/%s.png", class_name);

    // 載入點陣圖
    img = al_load_bitmap(buffer);

    // **關鍵檢查**：如果 img 是 NULL，代表載入失敗
    if (!img) {
        // 印出詳細的錯誤訊息，告訴你是哪個檔案路徑出了問題
        fprintf(stderr, "Failed to load bitmap from path: %s\n", buffer);
        return false; // 回傳 false 表示失敗
    }

    // 載入成功
    return true; // 回傳 true 表示成功
}

void
BandMember::Draw()
{
    if (img) {
        al_draw_bitmap(img, circle->x - circle->r, circle->y - circle->r, 0);
    }
    // 如果 img 是 NULL (因為載入失敗)，則不會執行任何繪圖操作，避免程式崩潰
}

bool
BandMember::Move()
{
    circle->x += speed * axis_x[direction];
    circle->y += speed * axis_y[direction];
    return false;
}

void
BandMember::changeDir(int dir) {
    switch(dir) {
        case 0: direction = LEFT; break;
        case 1: direction = RIGHT; break;
        case 2: direction = UP; break;
        case 3: direction = DOWN; break;
    }
}