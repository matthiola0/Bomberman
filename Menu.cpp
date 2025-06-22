#include "Menu.h"
#include <stdio.h>

const int ThumbWidth = 50;
const int ThumbHeight = 50;
const int gapX = 40, gapY = 30;
const int offsetX = 30 + field_width, offsetY = 150;
const int Initial_Health = 1;
const int Initial_Coin = 10;

bool
Menu::isInRange(int point, int startPos, int length)
{
    if(point >= startPos && point <= startPos + length)
        return true;

    return false;
}

Menu::Menu()
{
    char filename[50];

    HealthPoint = 10;
    Coin = 0;
    Score = 0;
    Time = initial_time;
    game_started = false;


    bocchi_img = al_load_bitmap("./Asset/bocchi_small.png");
    if (!bocchi_img) {
        fprintf(stderr, "FATAL ERROR in Menu.cpp: Failed to load bitmap './Asset/bocchi_small.png'\n");
        exit(1);
    }

    ijichi_img = al_load_bitmap("./Asset/ijichi_small.png");
    if (!ijichi_img) {
        fprintf(stderr, "FATAL ERROR in Menu.cpp: Failed to load bitmap './Asset/ijichi_small.png'\n");
        exit(1);
    }

    yamada_img = al_load_bitmap("./Asset/yamada_small.png");
    if (!yamada_img) {
        fprintf(stderr, "FATAL ERROR in Menu.cpp: Failed to load bitmap './Asset/yamada_small.png'\n");
        exit(1);
    }

    kita_img = al_load_bitmap("./Asset/kita_small.png");
    if (!kita_img) {
        fprintf(stderr, "FATAL ERROR in Menu.cpp: Failed to load bitmap './Asset/kita_small.png'\n");
        exit(1);
    }

    menuFont = al_load_ttf_font("pirulen.ttf", 12, 0);
    if (!menuFont) {
        fprintf(stderr, "FATAL ERROR in Menu.cpp: Failed to load font 'pirulen.ttf'\n");
        exit(1);
    }
}

Menu::~Menu()
{
    // al_destroy_bitmap(player1_img);
    // al_destroy_bitmap(player2_img);
    // al_destroy_bitmap(player3_img);
    // al_destroy_bitmap(player4_img);
    al_destroy_bitmap(bocchi_img);
    al_destroy_bitmap(ijichi_img);
    al_destroy_bitmap(yamada_img);
    al_destroy_bitmap(kita_img);
    al_destroy_font(menuFont);
}

void
Menu::Load(int player1_, int player2_, int player3_, int player4_) {
    
    player1 = player1_;
    player2 = player2_;
    player3 = player3_;
    player4 = player4_;

    switch (player1) {
        case BOCCHI:
            player1_img = bocchi_img;
            break;
        case IJICHI:
            player1_img = ijichi_img;
            break;
        case YAMADA:
            player1_img = yamada_img;
            break;
        case KITA:
            player1_img = kita_img;
            break;
    }
    switch (player2) {
        case BOCCHI:
            player2_img = bocchi_img;
            break;
        case IJICHI:
            player2_img = ijichi_img;
            break;
        case YAMADA:
            player2_img = yamada_img;
            break;
        case KITA:
            player2_img = kita_img;
            break;
    }
    switch (player3) {
        case BOCCHI:
            player3_img = bocchi_img;
            break;
        case IJICHI:
            player3_img = ijichi_img;
            break;
        case YAMADA:
            player3_img = yamada_img;
            break;
        case KITA:
            player3_img = kita_img;
            break;
    }
    switch (player4) {
        case BOCCHI:
            player4_img = bocchi_img;
            break;
        case IJICHI:
            player4_img = ijichi_img;
            break;
        case YAMADA:
            player4_img = yamada_img;
            break;
        case KITA:
            player4_img = kita_img;
            break;
    }
    game_started = true;
    // char buffer[50];
    // sprintf(buffer, "./Asset/%s_small.png", BandMemberClass[player1]);
    // player1_img = al_load_bitmap(buffer);
    // sprintf(buffer, "./Asset/%s_small.png", BandMemberClass[player2]);
    // player2_img = al_load_bitmap(buffer);
    // sprintf(buffer, "./Asset/%s_small.png", BandMemberClass[player3]);
    // player3_img = al_load_bitmap(buffer);
    // sprintf(buffer, "./Asset/%s_small.png", BandMemberClass[player4]);
    // player4_img = al_load_bitmap(buffer);


    menuFont = al_load_ttf_font("pirulen.ttf", 12, 0); // load font
    if (!menuFont) {
        fprintf(stderr, "FATAL ERROR: Failed to load font 'pirulen.ttf'\n");
        exit(1);
    }
}

void
Menu::Reset()
{
    HealthPoint = Initial_Health;
    Coin = Initial_Coin;
    Score = 0;
    Score1 = 0;   // player1
    Score2 = 0;   // player2
    Score3 = 0;   // player3
    Score4 = 0;   // player4
    Time = initial_time;   // test
    killedMonster = 0;
}

void
Menu::Draw()
{
    char buffer[50];
    if (!menuFont) {
        fprintf(stderr, "ERROR in Menu.cpp: menuFont is NULL\n");
        return;
    }
    // sprintf(buffer, "%d", HealthPoint);  // wait 炸彈數量 1 or 0
    // al_draw_bitmap(love, offsetX, 20, 0);   // wait 改炸彈圖片 存選擇的角色
    // al_draw_text(menuFont, al_map_rgb(255, 255, 255), 2*al_get_bitmap_width(love) + offsetX, 20, 0, buffer);

    sprintf(buffer, "Time: %d:%d", Time/60/60, Time/60%60);
    al_draw_text(menuFont, al_map_rgb(255, 255, 255), offsetX, 20 + gapY, 0, buffer);

    sprintf(buffer, ": %d", Score1);
    if (player1_img) { // 檢查指標是否有效
        al_draw_bitmap(player1_img, offsetX, 20 + 2*gapY, 0);
        al_draw_text(menuFont, al_map_rgb(255, 255, 255), al_get_bitmap_width(player1_img) + gapX/2 + offsetX, 20 + 2*gapY + 15, 0, buffer);
    }
    sprintf(buffer, ": %d", Score2);
    if (player2_img) { // 檢查指標是否有效
        al_draw_bitmap(player2_img, offsetX, 20 + 3*gapY + 15, 0);
        al_draw_text(menuFont, al_map_rgb(255, 255, 255), al_get_bitmap_width(player2_img) + gapX/2 + offsetX, 20 + 3*gapY + 30, 0, buffer);
    }
    sprintf(buffer, ": %d", Score3);
    if (player3_img) { // 檢查指標是否有效
        al_draw_bitmap(player3_img, offsetX, 20 + 5*gapY, 0);
        al_draw_text(menuFont, al_map_rgb(255, 255, 255), al_get_bitmap_width(player3_img) + gapX/2 + offsetX, 20 + 5*gapY + 15, 0, buffer);
    }
    sprintf(buffer, ": %d", Score4);
    if (player4_img) { // 檢查指標是否有效
        al_draw_bitmap(player4_img, offsetX, 20 + 6*gapY + 15, 0);
        al_draw_text(menuFont, al_map_rgb(255, 255, 255), al_get_bitmap_width(player4_img) + gapX/2 + offsetX, 20 + 6*gapY + 30, 0, buffer);
    }
}

bool
Menu::Subtract_HP(int escapeNum)
{
    HealthPoint -= escapeNum;

    return (HealthPoint == 0);
}

void
Menu::Gain_Score(int scoreWorth)
{
    killedMonster++;
    Score += scoreWorth;
}

bool 
Menu::win()
{
    if (Score1 < Score2)
        return false;
    if (Score1 < Score3)
        return false;
    if (Score1 < Score4)
        return false;
    
    return true;
}

// int
// Menu::MouseIn(int mouse_x, int mouse_y)
// {
//     bool enoughCoin;

//     selectedTower = -1;

//     for(int i=0; i < Num_TowerType; i++)
//     {
//         int pos_x = offsetX + (ThumbWidth + gapX) * (i % 2);
//         int pos_y = offsetY + (ThumbHeight + gapY) * (i / 2);

//         if(isInRange(mouse_x, pos_x, ThumbWidth) && isInRange(mouse_y, pos_y, ThumbHeight))
//         {
//             // if(Enough_Coin(i))
//             // {
//             //     selectedTower = i;
//             //     break;
//             // }
//         }
//     }
//     return selectedTower;
// }
