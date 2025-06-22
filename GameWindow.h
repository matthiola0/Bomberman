#ifndef MAINWINDOW_H_INCLUDED
#define MAINWINDOW_H_INCLUDED

#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

#include <vector>
#include <list>
#include <time.h>
#include "Menu.h"
#include "Level.h"
#include "Circle.h"
#include "global.h"
#include "Attack.h"
// #include "Slider.h"
// #include "bocchi.h"
// #include "ijichi.h"
// #include "yamada.h"
// #include "kita.h"
#include "BandMember.h"
// #include "Stone.h"

#define GAME_INIT -1
#define GAME_SETTING 0
#define GAME_SELECT 1
#define GAME_BEGIN 2
#define GAME_CONTINUE 3
#define GAME_FAIL 4
#define GAME_TERMINATE 5
#define GAME_NEXT_LEVEL 6
#define GAME_EXIT 7

// clock rate
const float FPS = 60;

// total number of level
const int LevelNum = 4;

// 1 coin every 2 seconds
const int CoinSpeed = FPS * 2;
const int Coin_Time_Gain = 1;


enum {MENU = 0, CHARCHOOSE, CHARSTORY, INTRODUCE, P1_P2, P2_P1, P2_P1BOCCHI, P2_P1YAMADA, P2_P1IJICHI, P2_P1KITA, MAPCHOOSE, GAMESCHOOL, GAMEHOME, GAMESTARRY, GAMEPAUSE, GAMEEND};    //                                            //    GAMEWINDOW                  // PAUSE


class GameWindow
{
public:
    // constructor
    GameWindow();
    // each process of scene
    void game_init();
    void game_reset();
    void game_play();
    void game_begin();

    int game_run();
    int game_update();

    void show_err_msg(int msg);
    void game_destroy();

    // each drawing scene function
    void draw_running_map(int);
    void draw_menu();
    void draw_charchoose();
    void draw_charstory();
    void draw_introduce();
    void draw_P1_P2();
    void draw_P2_P1();
    void draw_P2_P1bocchi();
    void draw_P2_P1ijichi();
    void draw_P2_P1kita();
    void draw_P2_P1yamada();
    void draw_mapchoose();
    void draw_pause();
    void draw_end();

    // process of updated event
    int process_event();
    // detect if mouse hovers over a rectangle
    bool mouse_hover(int, int, int, int);
    // detect if a bandmember will be on road
    // bool isOnRoad(int, int);

    // Tower* create_tower(int);
    // Monster* create_monster();
    // BandMember* create_bandmember();

public:
    bool initial = true;

private:
    ALLEGRO_BITMAP *icon;
    // ALLEGRO_BITMAP *tower[Num_TowerType];
    ALLEGRO_BITMAP *background1 = NULL;
    ALLEGRO_BITMAP *background2 = NULL;
    ALLEGRO_BITMAP *background3 = NULL;
    ALLEGRO_BITMAP *background_menu = NULL;
    ALLEGRO_BITMAP *background_charchoose = NULL;
    ALLEGRO_BITMAP *background_charstory = NULL;
    ALLEGRO_BITMAP *background_introduce = NULL;
    ALLEGRO_BITMAP *background_P1_P2 = NULL;
    ALLEGRO_BITMAP *background_P2_P1 = NULL;
    ALLEGRO_BITMAP *background_P2_P1bocchi = NULL;
    ALLEGRO_BITMAP *background_P2_P1yamada = NULL;
    ALLEGRO_BITMAP *background_P2_P1ijichi = NULL;
    ALLEGRO_BITMAP *background_P2_P1kita = NULL;
    ALLEGRO_BITMAP *background_mapchoose = NULL;
    ALLEGRO_BITMAP *background_pause = NULL;
    ALLEGRO_BITMAP *background_win = NULL;
    ALLEGRO_BITMAP *background_lose = NULL;
    ALLEGRO_BITMAP *speedtool = NULL;

    // ALLEGRO_BITMAP *wall = NULL;
    // ALLEGRO_BITMAP *stone = NULL;

    ALLEGRO_BITMAP *wall_school;
    ALLEGRO_BITMAP *stone_school;
    ALLEGRO_BITMAP *wall_home;
    ALLEGRO_BITMAP *stone_home;
    ALLEGRO_BITMAP *wall_starry;
    ALLEGRO_BITMAP *stone_starry;

    ALLEGRO_BITMAP* load_bitmap_safely(const char* path);
    ALLEGRO_FONT* load_font_safely(const char* path, int size, int flags);


    ALLEGRO_DISPLAY* display = NULL;
    ALLEGRO_FONT *font = NULL;
    ALLEGRO_FONT *Medium_font = NULL;
    ALLEGRO_FONT *Large_font = NULL;

    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_EVENT event;
    ALLEGRO_TIMER *timer = NULL;
    // ALLEGRO_TIMER *monster_pro = NULL;

    ALLEGRO_SAMPLE *sample = NULL;
    ALLEGRO_SAMPLE_INSTANCE *startSound = NULL;
    ALLEGRO_SAMPLE_INSTANCE *clearSound = NULL;
    ALLEGRO_SAMPLE_INSTANCE *failSound = NULL;
    ALLEGRO_SAMPLE_INSTANCE *backgroundSound = NULL;

    // ALLEGRO_BITMAP *member1 = NULL;


    LEVEL *level = NULL;
    Menu *menu = NULL;

    // std::vector<Monster*> monsterSet;
    // std::list<Tower*> towerSet;


    std::vector<BandMember*> bandmemberSet;
    // bocchi* bocchi_charact;
    // ijichi* ijichi_charact;
    // yamada* yamada_charact;
    // kita* kita_charact;

    int player1;
    int player2;    // player or computer
    int player3;    // computer
    int player4;    // computer

    bool two_player = true;

    std::vector<Bomb*> bombSet;

    // int Monster_Pro_Count = 0;
    int Time_Inc_Count = 0;
    // int Coin_Inc_Count = 0;
    int mouse_x, mouse_y;
    // int selectedTower = -1, lastClicked = -1;

    bool redraw = false;
    bool mute = false;

    bool moving1 = false;
    bool moving2 = false;
    bool moving3 = false;
    bool moving4 = false;

    int cur_scene;
    int scene;
};


#endif // MAINWINDOW_H_INCLUDED
