#include "GameWindow.h"
#include "global.h"
#include <iostream>

#define WHITE al_map_rgb(255, 255, 255)
#define BLACK al_map_rgb(0, 0, 0)
#define PURPLE al_map_rgb(233, 157, 185)
#define YELLOW al_map_rgb(238, 220, 120)
#define BLUE al_map_rgb(51, 85, 165)
#define RED al_map_rgb(220, 58, 48)

#define min(a, b) ((a) < (b)? (a) : (b))
#define max(a, b) ((a) > (b)? (a) : (b))


float Attack::volume = 1.0;

void set_attack_volume(float volume)
{
    Attack::volume = volume;
}

// bool compare(Tower *t1, Tower *t2)
// {
//     return (t1->getY() <= t2->getY());
// }

ALLEGRO_BITMAP* GameWindow::load_bitmap_safely(const char* path)
{
    ALLEGRO_BITMAP* bmp = al_load_bitmap(path);
    if (!bmp) {
        // 如果載入失敗，直接顯示錯誤訊息並終止遊戲
        fprintf(stderr, "Fatal Error: Failed to load bitmap: %s\n", path);
        show_err_msg(GAME_TERMINATE);
    }
    return bmp;
}

ALLEGRO_FONT* GameWindow::load_font_safely(const char* path, int size, int flags)
{
    ALLEGRO_FONT* font = al_load_ttf_font(path, size, flags);
    if (!font) {
        // 如果載入失敗，直接顯示錯誤訊息並終止遊戲
        fprintf(stderr, "Fatal Error: Failed to load font: %s\n", path);
        show_err_msg(GAME_TERMINATE);
    }
    return font;
}

void
GameWindow::game_init()
{
    // char buffer[50];

    icon = load_bitmap_safely("./icon.png");
    background1 = load_bitmap_safely("./Asset/school.jpg");
    background2 = load_bitmap_safely("./Asset/home.jpg");
    background3 = load_bitmap_safely("./Asset/starry.png");
    background_menu = load_bitmap_safely("./Asset/menu.png");
    background_charchoose = load_bitmap_safely("./Asset/characterchoose.png");
    background_charstory = load_bitmap_safely("./Asset/characterstory.png");
    background_introduce = load_bitmap_safely("./Asset/introduce.png");
    background_pause = load_bitmap_safely("./Asset/pause.png");
    background_win = load_bitmap_safely("./Asset/win.png");
    background_lose = load_bitmap_safely("./Asset/lose.png");
    background_P1_P2 = load_bitmap_safely("./Asset/P1_P2.png");
    background_P2_P1 = load_bitmap_safely("./Asset/P2_P1.png");
    background_P2_P1bocchi = load_bitmap_safely("./Asset/P2_P1bocchi.png");
    background_P2_P1ijichi = load_bitmap_safely("./Asset/P2_P1ijichi.png");
    background_P2_P1kita = load_bitmap_safely("./Asset/P2_P1kita.png");
    background_P2_P1yamada = load_bitmap_safely("./Asset/P2_P1yamada.png");
    background_mapchoose = load_bitmap_safely("./Asset/mapchoose.png");
    speedtool = load_bitmap_safely("./Asset/speedtool.png");

    // *** 將所有地圖物件一次性載入 ***
    wall_school = load_bitmap_safely("./Asset/table.png");
    stone_school = load_bitmap_safely("./Asset/chair.png");
    wall_home = load_bitmap_safely("./Asset/homewall.png");
    stone_home = load_bitmap_safely("./Asset/homestone.png");
    wall_starry = load_bitmap_safely("./Asset/starrywall.png");
    stone_starry = load_bitmap_safely("./Asset/starrystone.png");

    // wall = al_load_bitmap("./Asset/table.png");
    // stone = al_load_bitmap("./Asset/chair.png");

    al_set_display_icon(display, icon);
    al_reserve_samples(3);


    sample = al_load_sample("bocchi_song.mp3");
    if (!sample) {
        fprintf(stderr, "Fatal Error: Failed to load audio sample: bocchi_song.mp3\n");
        show_err_msg(GAME_TERMINATE);
    }
    backgroundSound = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(backgroundSound, ALLEGRO_PLAYMODE_LOOP);
    al_attach_sample_instance_to_mixer(backgroundSound, al_get_default_mixer());

    bandmemberSet.clear();
    bombSet.clear();


    player1 = BOCCHI;
    player2 = IJICHI;
    player3 = YAMADA;
    player4 = KITA;

    two_player = true;

    level = new LEVEL(1);
    menu = new Menu();

    scene = MENU;

    bandmemberSet.push_back(new BandMember(BOCCHI));
    bandmemberSet.push_back(new BandMember(IJICHI));
    bandmemberSet.push_back(new BandMember(YAMADA));
    bandmemberSet.push_back(new BandMember(KITA));
}

bool
GameWindow::mouse_hover(int startx, int starty, int width, int height)
{
    if(mouse_x >= startx && mouse_x <= startx + width)
        if(mouse_y >= starty && mouse_y <= starty + height)
            return true;

    return false;
}

void
GameWindow::game_play()
{
    int msg;

    srand(time(NULL));

    msg = -1;
    game_reset();
    game_begin();

    while(msg != GAME_EXIT)
    {
        msg = game_run();
    }

    show_err_msg(msg);
}

void
GameWindow::show_err_msg(int msg)
{
    if(msg == GAME_TERMINATE)
        fprintf(stderr, "Game Terminated...");
    else
        fprintf(stderr, "unexpected msg: %d", msg);

    game_destroy();
    exit(9);
}

GameWindow::GameWindow()
{
    if (!al_init())
        show_err_msg(-1);

    printf("Game Initializing...\n");

    display = al_create_display(window_width, window_height);
    event_queue = al_create_event_queue();

    timer = al_create_timer(1.0 / FPS);
    // monster_pro = al_create_timer(1.0 / FPS);

    // if(timer == NULL || monster_pro == NULL)
    //     show_err_msg(-1);
    if(timer == NULL)
        show_err_msg(-1);

    if (display == NULL || event_queue == NULL)
        show_err_msg(-1);

    al_init_primitives_addon();
    al_init_font_addon(); // initialize the font addon
    al_init_ttf_addon(); // initialize the ttf (True Type Font) addon
    al_init_image_addon(); // initialize the image addon
    al_init_acodec_addon(); // initialize acodec addon

    al_install_keyboard(); // install keyboard event
    al_install_mouse();    // install mouse event
    al_install_audio();    // install audio event

    font = load_font_safely("Caviar_Dreams_Bold.ttf", 12, 0);
    Medium_font = load_font_safely("Caviar_Dreams_Bold.ttf", 24, 0);
    Large_font = load_font_safely("Caviar_Dreams_Bold.ttf", 36, 0);

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());

    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    // al_register_event_source(event_queue, al_get_timer_event_source(monster_pro));

    game_init();
}

void
GameWindow::game_begin()
{
    // printf(">>> Start Level[%d]\n", level->getLevel());

    draw_menu();

    // draw_running_map(GAMESCHOOL);

    // al_play_sample_instance(startSound);
    // while(al_get_sample_instance_playing(startSound));
    al_play_sample_instance(backgroundSound);

    al_start_timer(timer);
    // al_start_timer(monster_pro);
}

int
GameWindow::game_run()
{
    int error = GAME_CONTINUE;

    if (!al_is_event_queue_empty(event_queue)) {

        error = process_event();
    }
    return error;
}

int
GameWindow::game_update() 
{
    //test if is legal
    //player1
    int dir = bandmemberSet[player1]->getDir();
    int x_now = bandmemberSet[player1]->getX();
    int y_now = bandmemberSet[player1]->getY();
    int x_next = (x_now+axis_x[dir]);
    int y_next = (y_now+axis_y[dir]);

    int target1 = (x_next/grid_width + y_next/grid_height * 15);
    int target2 = (x_next/grid_width + (y_next+member_width-1)/grid_height * 15);
    int target3 = ((x_next+member_width-1)/grid_width + y_next/grid_height * 15);
    int target4 = ((x_next+member_width-1)/grid_width + (y_next+member_width-1)/grid_height * 15);
        
    if (level->have_speed(target1) && target1 == target2 && target2 == target3 && target3 == target4) {
        level->speed_disappear(target1);
        bandmemberSet[player1]->SpeedChange(2.5);
    }
    if (level->is_bombing(target1) || level->is_bombing(target2) || level->is_bombing(target3) || level->is_bombing(target4)) {
        level->clear_color(player1);
    } 
    else if (moving1 && level->isRoad(target1) && level->isRoad(target2) && level->isRoad(target3) && level->isRoad(target4))
        bandmemberSet[player1]->Move();

    //player2
    dir = bandmemberSet[player2]->getDir();
    x_now = bandmemberSet[player2]->getX();
    y_now = bandmemberSet[player2]->getY();
    x_next = (x_now+axis_x[dir]);
    y_next = (y_now+axis_y[dir]);

    target1 = (x_next/grid_width + y_next/grid_height * 15);
    target2 = (x_next/grid_width + (y_next+member_width-1)/grid_height * 15);
    target3 = ((x_next+member_width-1)/grid_width + y_next/grid_height * 15);
    target4 = ((x_next+member_width-1)/grid_width + (y_next+member_width-1)/grid_height * 15);
        
    if (level->have_speed(target1) && target1 == target2 && target2 == target3 && target3 == target4) {
        level->speed_disappear(target1);
        bandmemberSet[player2]->SpeedChange(2.5);
    }
    if (level->is_bombing(target1) || level->is_bombing(target2) || level->is_bombing(target3) || level->is_bombing(target4)) {
        if (two_player)
            level->clear_color(player1);
    }
    else if (moving2 && level->isRoad(target1) && level->isRoad(target2) && level->isRoad(target3) && level->isRoad(target4))
        bandmemberSet[player2]->Move();

    //player3
    dir = bandmemberSet[player3]->getDir();
    x_now = bandmemberSet[player3]->getX();
    y_now = bandmemberSet[player3]->getY();
    x_next = (x_now+axis_x[dir]);
    y_next = (y_now+axis_y[dir]);

    target1 = (x_next/grid_width + y_next/grid_height * 15);
    target2 = (x_next/grid_width + (y_next+member_width-1)/grid_height * 15);
    target3 = ((x_next+member_width-1)/grid_width + y_next/grid_height * 15);
    target4 = ((x_next+member_width-1)/grid_width + (y_next+member_width-1)/grid_height * 15);
        
    if (level->have_speed(target1) && target1 == target2 && target2 == target3 && target3 == target4) {
        level->speed_disappear(target1);
        bandmemberSet[player3]->SpeedChange(2.5);
    }
    if (level->is_bombing(target1) || level->is_bombing(target2) || level->is_bombing(target3) || level->is_bombing(target4)) {}        // wait
    else if (moving3 && level->isRoad(target1) && level->isRoad(target2) && level->isRoad(target3) && level->isRoad(target4))
        bandmemberSet[player3]->Move();

    //player4
    dir = bandmemberSet[player4]->getDir();
    x_now = bandmemberSet[player4]->getX();
    y_now = bandmemberSet[player4]->getY();
    x_next = (x_now+axis_x[dir]);
    y_next = (y_now+axis_y[dir]);

    target1 = (x_next/grid_width + y_next/grid_height * 15);
    target2 = (x_next/grid_width + (y_next+member_width-1)/grid_height * 15);
    target3 = ((x_next+member_width-1)/grid_width + y_next/grid_height * 15);
    target4 = ((x_next+member_width-1)/grid_width + (y_next+member_width-1)/grid_height * 15);
        
    if (level->have_speed(target1) && target1 == target2 && target2 == target3 && target3 == target4) {
        level->speed_disappear(target1);
        bandmemberSet[player4]->SpeedChange(2.5);
    }
    if (level->is_bombing(target1) || level->is_bombing(target2) || level->is_bombing(target3) || level->is_bombing(target4)) {}        // wait
    else if (moving4 && level->isRoad(target1) && level->isRoad(target2) && level->isRoad(target3) && level->isRoad(target4))
        bandmemberSet[player4]->Move();
    

    return GAME_CONTINUE;
}

void
GameWindow::game_reset()
{
    bandmemberSet.clear();
    bombSet.clear();


    Time_Inc_Count = 0;
    // Monster_Pro_Count = 0;
    mute = false;
    redraw = false;
    menu->Reset();

    player1 = BOCCHI;
    player2 = IJICHI;
    player3 = YAMADA;
    player4 = KITA;

    // stop sample instance
    al_stop_sample_instance(backgroundSound);
    // al_stop_sample_instance(startSound);

    // stop timer
    al_stop_timer(timer);
    // al_stop_timer(monster_pro);
}

void
GameWindow::game_destroy()
{
    game_reset();

    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_font(font);
    al_destroy_font(Medium_font);
    al_destroy_font(Large_font);

    al_destroy_timer(timer);
    // al_destroy_timer(monster_pro);

    // for(int i=0;i<5; i++)
    //     al_destroy_bitmap(tower[i]);

    al_destroy_bitmap(icon);
    al_destroy_bitmap(background1);
    al_destroy_bitmap(background2);
    al_destroy_bitmap(background3);
    al_destroy_bitmap(background_menu);
    al_destroy_bitmap(background_charchoose);
    al_destroy_bitmap(background_charstory);
    al_destroy_bitmap(background_introduce);
    al_destroy_bitmap(background_pause);
    al_destroy_bitmap(background_win);
    al_destroy_bitmap(background_lose);
    al_destroy_bitmap(background_P1_P2);
    al_destroy_bitmap(background_P2_P1);
    al_destroy_bitmap(background_P2_P1bocchi);
    al_destroy_bitmap(background_P2_P1ijichi);
    al_destroy_bitmap(background_P2_P1kita);
    al_destroy_bitmap(background_P2_P1yamada);
    al_destroy_bitmap(background_mapchoose);
    al_destroy_bitmap(speedtool);

    al_destroy_bitmap(wall_school);
    al_destroy_bitmap(stone_school);
    al_destroy_bitmap(wall_home);
    al_destroy_bitmap(stone_home);
    al_destroy_bitmap(wall_starry);
    al_destroy_bitmap(stone_starry);


    al_destroy_sample(sample);
    // al_destroy_sample_instance(startSound);
    al_destroy_sample_instance(backgroundSound);

    delete level;
    delete menu;
}

int
GameWindow::process_event()
{
    int i;
    int instruction = GAME_CONTINUE;

    // offset for pause window
    int offsetX = field_width/2 - 200;
    int offsetY = field_height/2 - 200;

    al_wait_for_event(event_queue, &event);
    redraw = false;

    if(event.type == ALLEGRO_EVENT_TIMER) {
        if(event.timer.source == timer) {
            redraw = true;

            if (menu->if_gaame_started())
            {
                // fprintf(stderr, "DEBUG: Game has started, updating time. Current Time = %d\n", menu->getTime());

                int randomNum = rand() % 320;
                if (randomNum == 0) {
                    bandmemberSet[player3]->changeDir(2); // move up
                    moving3 = true;
                } else if (randomNum == 1) {
                    bandmemberSet[player3]->changeDir(3); // move down
                    moving3 = true;
                } else if (randomNum == 2) {
                    bandmemberSet[player3]->changeDir(0); // move left
                    moving3 = true;
                } else if (randomNum == 3) {
                    bandmemberSet[player3]->changeDir(1); // move right
                }
                else if (randomNum == 4) {
                    Bomb* bomb = new Bomb(bandmemberSet[player3]->getX(), bandmemberSet[player3]->getY(), BandMemberClass[player3]);
                    bombSet.push_back(bomb);
                }

                randomNum = rand() % 320;
                if (randomNum == 0) {
                    bandmemberSet[player4]->changeDir(2); // move up
                    moving4 = true;
                } else if (randomNum == 1) {
                    bandmemberSet[player4]->changeDir(3); // move down
                    moving4 = true;
                } else if (randomNum == 2) {
                    bandmemberSet[player4]->changeDir(0); // move left
                    moving4 = true;
                } else if (randomNum == 3) {
                    bandmemberSet[player4]->changeDir(1); // move right
                }
                else if (randomNum == 4) {
                    Bomb* bomb = new Bomb(bandmemberSet[player4]->getX(), bandmemberSet[player4]->getY(), BandMemberClass[player4]);
                    bombSet.push_back(bomb);
                }

                if (!two_player) {
                    randomNum = rand() % 320;
                    if (randomNum == 0) {
                        bandmemberSet[player2]->changeDir(2); // move up
                        moving2 = true;
                    } else if (randomNum == 1) {
                        bandmemberSet[player2]->changeDir(3); // move down
                        moving2 = true;
                    } else if (randomNum == 2) {
                        bandmemberSet[player2]->changeDir(0); // move left
                        moving2 = true;
                    } else if (randomNum == 3) {
                        bandmemberSet[player2]->changeDir(1); // move right
                    }
                    else if (randomNum == 4) {
                        Bomb* bomb = new Bomb(bandmemberSet[player2]->getX(), bandmemberSet[player2]->getY(), BandMemberClass[player2]);
                        bombSet.push_back(bomb);
                    }
                }

                menu->Change_Time();
            }

            for (int i = BOCCHI; i <= KITA; i++) {
                bandmemberSet[i]->change_counter();

                if (bandmemberSet[i]->getSCounter() <= 0)
                    bandmemberSet[i]->SpeedChange(1);
            }

            for (int i=0; i<bombSet.size(); i++)
            {
                bombSet[i]->change_counter();
            }
        }
    }
    else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        return GAME_EXIT;
    }
    else if(event.type == ALLEGRO_EVENT_KEY_DOWN && scene >= GAMESCHOOL && scene <= GAMESTARRY) {
        Bomb *bomb;
        switch(event.keyboard.keycode) {   
            case ALLEGRO_KEY_P:
                /*TODO: handle pause event here*/
                if(al_get_timer_started(timer)) {
                    al_stop_timer(timer);
                    cur_scene = scene;
                    scene = GAMEPAUSE;
                    redraw = true;
                    // printf("PASS\n");
                }
                else{
                    scene = cur_scene;
                    al_start_timer(timer);
                }
                break;
            case ALLEGRO_KEY_M:
                mute = !mute;
                if(mute)
                    al_stop_sample_instance(backgroundSound);
                else
                    al_play_sample_instance(backgroundSound);
                break;
            case ALLEGRO_KEY_W:
                bandmemberSet[player1]->changeDir(2);
                moving1 = true;
                break;
            case ALLEGRO_KEY_S:
                bandmemberSet[player1]->changeDir(3);
                moving1 = true;
                break;
            case ALLEGRO_KEY_A:
                bandmemberSet[player1]->changeDir(0);
                moving1 = true;
                break;
            case ALLEGRO_KEY_D:
                bandmemberSet[player1]->changeDir(1);
                moving1 = true;
                break;
            case ALLEGRO_KEY_SPACE:
                bomb = new Bomb(bandmemberSet[player1]->getX(), bandmemberSet[player1]->getY(), BandMemberClass[player1]);
                bombSet.push_back(bomb);
                break;
            case ALLEGRO_KEY_UP:
                if (two_player) {
                    bandmemberSet[player2]->changeDir(2);
                    moving2 = true;
                }
                break;
            case ALLEGRO_KEY_DOWN:
                if (two_player) {
                    bandmemberSet[player2]->changeDir(3);
                    moving2 = true;
                }
                break;
            case ALLEGRO_KEY_LEFT:
                if (two_player) {
                    bandmemberSet[player2]->changeDir(0);
                    moving2 = true;
                }
                break;
            case ALLEGRO_KEY_RIGHT:
                if (two_player) {
                    bandmemberSet[player2]->changeDir(1);
                    moving2 = true;
                }
                break;
            case ALLEGRO_KEY_ENTER:
                if (two_player) {
                    bomb = new Bomb(bandmemberSet[player2]->getX(), bandmemberSet[player2]->getY(), BandMemberClass[player2]);
                    bombSet.push_back(bomb);
                }
                break;
        }
    }
    else if(event.type == ALLEGRO_EVENT_KEY_UP && scene >= GAMESCHOOL && scene <= GAMESTARRY) {
        switch(event.keyboard.keycode) {
            case ALLEGRO_KEY_W:
                moving1 = false;
                break;
            case ALLEGRO_KEY_S:
                moving1 = false;
                break;
            case ALLEGRO_KEY_A:
                moving1 = false;
                break;
            case ALLEGRO_KEY_D:
                moving1 = false;
                break;
            case ALLEGRO_KEY_UP:
                moving2 = false;
                break;
            case ALLEGRO_KEY_DOWN:
                moving2 = false;
                break;
            case ALLEGRO_KEY_LEFT:
                moving2 = false;
                break;
            case ALLEGRO_KEY_RIGHT:
                moving2 = false;
                break;
        }
    }
    else if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
        if(event.mouse.button == 1) {
            switch (scene) {
                case MENU:
                    if(mouse_hover(69, 339,  159-69, 388-339))
                        scene = P1_P2;
                    else if(mouse_hover(232, 341,  354-232, 388-341))
                        scene = INTRODUCE;
                    else if(mouse_hover(441, 346,  560-441, 388-346))
                        scene = CHARSTORY;
                    else if(mouse_hover(659, 346,  736-659, 388-339))
                        return GAME_EXIT;
                    break;
                case INTRODUCE:
                    if(mouse_hover(20, 400,  195-20, 575-400))
                        scene = MENU;
                    break;
                case CHARCHOOSE:
                    if(mouse_hover(34, 39,  104-34, 106-39))
                        scene = MENU;
                    else {
                        if(mouse_hover(69, 364,  165-69, 400-364)) {
                            player1 = BOCCHI;
                            player2 = IJICHI;
                            player3 = YAMADA;
                            player4 = KITA;
                            menu->Load(BOCCHI, IJICHI, YAMADA, KITA);
                            scene = MAPCHOOSE;
                        }
                        else if(mouse_hover(232, 364,  354-232, 400-364)) {
                            player1 = YAMADA;
                            player2 = IJICHI;
                            player3 = BOCCHI;
                            player4 = KITA;
                            menu->Load(YAMADA, IJICHI, BOCCHI, KITA);
                            scene = MAPCHOOSE;
                        }
                        else if(mouse_hover(441, 364,  560-441, 400-364)) {
                            player1 = IJICHI;
                            player2 = BOCCHI;
                            player3 = YAMADA;
                            player4 = KITA;
                            menu->Load(IJICHI, BOCCHI, YAMADA, KITA);
                            scene = MAPCHOOSE;
                        }
                        else if(mouse_hover(659, 364,  736-659, 400-364)) {
                            player1 = KITA;
                            player2 = IJICHI;
                            player3 = YAMADA;
                            player4 = BOCCHI;
                            menu->Load(KITA, IJICHI, YAMADA, BOCCHI);
                            scene = MAPCHOOSE;
                        }          
                        // scene = MAPCHOOSE;
                    }
                    break;
                case CHARSTORY:
                    if(mouse_hover(19, 516,  94-19, 589-516))
                        scene = MENU;
                    break;
                case GAMEPAUSE:
                    if(mouse_hover(295, 194,  794-295, 341-194)) {
                        scene = cur_scene;
                        al_start_timer(timer);
                    }
                    else if(mouse_hover(295, 362,  794-295, 487-362)) {
                        scene = MENU;
                        al_start_timer(timer);
                    }
                    break;
                case P1_P2:
                    if(mouse_hover(0,0,400,600)) {
                        scene = CHARCHOOSE;
                        two_player = false;
                    }
                    else if(mouse_hover(400,0,400,600)) {
                        scene = P2_P1;
                        two_player = true;
                    }
                    break;
                case P2_P1:
                    if(mouse_hover(34, 39,  104-34, 106-39))
                        scene = MENU;
                    else if(mouse_hover(69,364,165-69,400-364)) {
                        player1 = BOCCHI;
                        scene = P2_P1BOCCHI;
                    }
                    else if(mouse_hover(275,364,360-275,400-364)) {
                        player1 = YAMADA;
                        scene = P2_P1YAMADA;
                    }
                    else if(mouse_hover(466,364,584-466,400-364)) {
                        player1 = IJICHI;
                        scene = P2_P1IJICHI;
                    }
                    else if(mouse_hover(666,364,761-666,400-364)) {
                        player1 = KITA;
                        scene = P2_P1KITA;
                    }
                    break;
                case P2_P1BOCCHI:
                    if(mouse_hover(34, 39,  104-34, 106-39))
                        scene = P2_P1;
                    else if(mouse_hover(275,364,360-275,400-364)) {
                        player2 = YAMADA;
                        player3 = IJICHI;
                        player4 = KITA;
                        menu->Load(BOCCHI, YAMADA, IJICHI, KITA);
                        scene = MAPCHOOSE;
                    }
                    else if(mouse_hover(466,364,584-466,400-364)) {
                        player2 = IJICHI;
                        player3 = YAMADA;
                        player4 = KITA;
                        menu->Load(BOCCHI, IJICHI, YAMADA, KITA);
                        scene = MAPCHOOSE;
                    }
                    else if(mouse_hover(666,364,761-666,400-364)) {
                        player2 = KITA;
                        player3 = IJICHI;
                        player4 = YAMADA;
                        menu->Load(BOCCHI, KITA, IJICHI, YAMADA);
                        scene = MAPCHOOSE;
                    }
                    break;
                case P2_P1YAMADA:
                    if(mouse_hover(34, 39,  104-34, 106-39))
                        scene = P2_P1;
                    else if(mouse_hover(69,364,165-69,400-364)) {
                        player2 = BOCCHI;
                        player3 = IJICHI;
                        player4 = KITA;
                        menu->Load(YAMADA, BOCCHI, IJICHI, KITA);
                        scene = MAPCHOOSE;
                        
                    }
                    else if(mouse_hover(466,364,584-466,400-364)) {
                        player2 = IJICHI;
                        player3 = BOCCHI;
                        player4 = KITA;
                        menu->Load(YAMADA, IJICHI, BOCCHI, KITA);
                        scene = MAPCHOOSE;
                    }
                    else if(mouse_hover(666,364,761-666,400-364)) {
                        player2 = KITA;
                        player3 = IJICHI;
                        player4 = BOCCHI;
                        menu->Load(YAMADA, KITA, IJICHI, BOCCHI);
                        scene = MAPCHOOSE;
                    }
                    break;
                case P2_P1IJICHI:
                    if(mouse_hover(34, 39,  104-34, 106-39))
                        scene = P2_P1;
                    else if(mouse_hover(69,364,165-69,400-364)) {
                        player2 = BOCCHI;
                        player3 = YAMADA;
                        player4 = KITA;
                        menu->Load(IJICHI, BOCCHI, YAMADA, KITA);
                        scene = MAPCHOOSE;
                    }
                    else if(mouse_hover(275,364,360-275,400-364)) {
                        player2 = YAMADA;
                        player3 = BOCCHI;
                        player4 = KITA;
                        menu->Load(IJICHI, YAMADA, BOCCHI, KITA);
                        scene = MAPCHOOSE;
                    }
                    else if(mouse_hover(666,364,761-666,400-364)) {
                        player2 = KITA;
                        player3 = YAMADA;
                        player4 = BOCCHI;
                        menu->Load(IJICHI, KITA, YAMADA, BOCCHI);
                        scene = MAPCHOOSE;
                    }
                    break;
                case P2_P1KITA:
                    if(mouse_hover(34, 39,  104-34, 106-39))
                        scene = P2_P1;
                    else if(mouse_hover(69,364,165-69,400-364)) {
                        player2 = BOCCHI;
                        player3 = IJICHI;
                        player4 = YAMADA;
                        menu->Load(KITA, BOCCHI, IJICHI, YAMADA);
                        scene = MAPCHOOSE;
                    }
                    else if(mouse_hover(275,364,360-275,400-364)) {
                        player2 = YAMADA;
                        player3 = IJICHI;
                        player4 = BOCCHI;
                        menu->Load(KITA, YAMADA, IJICHI, BOCCHI);
                        scene = MAPCHOOSE;
                    }
                    else if(mouse_hover(466,364,584-466,400-364)) {
                        player2 = IJICHI;
                        player3 = BOCCHI;
                        player4 = YAMADA;
                        menu->Load(KITA, IJICHI, BOCCHI, YAMADA);
                        scene = MAPCHOOSE;
                    }
                    break;
                case MAPCHOOSE:
                    if(mouse_hover(0,0,800,200)) {
                        menu->Reset();
                        for (int it = BOCCHI; it <= KITA; it++) {
                            bandmemberSet[it]->Reset();
                        }
                        level->Reset(1);
                        scene = GAMESCHOOL;
                    }
                    else if(mouse_hover(0,200,800,200)) {
                        menu->Reset();
                        for (int it = BOCCHI; it <= KITA; it++) {
                            bandmemberSet[it]->Reset();
                        }
                        level->Reset(2);
                        scene = GAMEHOME;
                    }
                    else if(mouse_hover(0,400,800,200)) {
                        menu->Reset();
                        for (int it = BOCCHI; it <= KITA; it++) {
                            bandmemberSet[it]->Reset();
                        }
                        level->Reset(3);
                        scene = GAMESTARRY;
                    }
                case GAMEEND:
                    if (mouse_hover(0, 540,  188-0, 580-540)) 
                        scene = MENU;
                    else if (mouse_hover(694, 540,  785-694, 580-540))
                        return GAME_EXIT;
                    break;
            }
        }
    }
    else if(event.type == ALLEGRO_EVENT_MOUSE_AXES){
        mouse_x = event.mouse.x;
        mouse_y = event.mouse.y;

        // menu->MouseIn(mouse_x, mouse_y);
    }

    if(redraw) {
        // update each object in game
        instruction = game_update();

        // Re-draw map
        switch (scene) {
            case MENU:
                draw_menu();
                break;
            case CHARCHOOSE:
                draw_charchoose();
                break;
            case CHARSTORY:
                draw_charstory();
                break;
            case INTRODUCE:
                draw_introduce();
                break;
            case P1_P2:
                draw_P1_P2();
                break;
            case P2_P1:
                draw_P2_P1();
                break;
            case P2_P1BOCCHI:
                draw_P2_P1bocchi();
                break;
            case P2_P1YAMADA:
                draw_P2_P1yamada();
                break;
            case P2_P1IJICHI:
                draw_P2_P1ijichi();
                break;
            case P2_P1KITA:
                draw_P2_P1kita();
                break;
            case MAPCHOOSE:
                draw_mapchoose();
                break;
            case GAMESCHOOL:
                if (menu->getTime() <= 0) {
                    scene = GAMEEND; 
                }
                draw_running_map(GAMESCHOOL);
                break;
            case GAMEHOME:
                if (menu->getTime() <= 0) {
                    scene = GAMEEND; 
                }
                draw_running_map(GAMEHOME);
                break;
            case GAMESTARRY:
                if (menu->getTime() <= 0) {
                    scene = GAMEEND; 
                }
                draw_running_map(GAMESTARRY);
                break;
            case GAMEPAUSE:
                draw_pause();
                break;
            case GAMEEND:
                draw_end();
                break;
        }
        redraw = false;
    }

    if(scene == GAMEPAUSE) {
        draw_pause();
    }

    return instruction;
}

void
GameWindow::draw_running_map(int scene)
{
    unsigned int i, j;
    ALLEGRO_BITMAP *current_wall = NULL;
    ALLEGRO_BITMAP *current_stone = NULL;
    
    al_clear_to_color(al_map_rgb(100, 100, 100));

    // --- DEBUG START ---
    // fprintf(stderr, "DEBUG: In draw_running_map for scene %d.\n", scene);

    switch (scene) {
        case GAMESCHOOL:
            // fprintf(stderr, "DEBUG: Attempting to draw 'background1'.\n");
            al_draw_bitmap(background1, 0, 0, 0);
            // fprintf(stderr, "DEBUG: OK.\n");
            current_wall = wall_school;
            current_stone = stone_school;
            break;
        case GAMEHOME:
            // fprintf(stderr, "DEBUG: Attempting to draw 'background2'.\n");
            al_draw_bitmap(background2, 0, 0, 0);
            // fprintf(stderr, "DEBUG: OK.\n");
            current_wall = wall_home;
            current_stone = stone_home;
            break;
        case GAMESTARRY:
            // fprintf(stderr, "DEBUG: Attempting to draw 'background3'.\n");
            al_draw_bitmap(background3, 0, 0, 0);
            // fprintf(stderr, "DEBUG: OK.\n");
            current_wall = wall_starry;
            current_stone = stone_starry;
            break;
    }

    int Score1 = 0, Score2 = 0, Score3 = 0, Score4 = 0;
    // draw map
    for(i = 0; i < field_height/40; i++)
    {
        for(j = 0; j < field_width/40; j++)
        {
            // char buffer[50];
            int tmp = i*15+j;
            // sprintf(buffer, "%d", tmp);
            if(level->isStone(tmp)) {
                // fprintf(stderr, "DEBUG: Attempting to draw 'stone' at %d,%d.\n", j, i);
                if (current_stone) al_draw_bitmap(current_stone, j*40-20, i*40-20, 0);
                // fprintf(stderr, "DEBUG: OK.\n");
            }
            else if(level->isRoad(i*15 + j)) {
                if (!(level->haveColor(tmp))) {
                    al_draw_filled_rectangle(j*40-20, i*40-20, j*40+20, i*40+20, WHITE);
                }
                else {
                    switch (level->get_character(tmp))
                    {
                        case BOCCHI:
                            Score1 += 50 * (player1 == BOCCHI);
                            Score2 += 50 * (player2 == BOCCHI);
                            Score3 += 50 * (player3 == BOCCHI);
                            Score4 += 50 * (player4 == BOCCHI);
                            al_draw_filled_rectangle(j*40-20, i*40-20, j*40+20, i*40+20, PURPLE);
                            break;
                        case IJICHI:
                            Score1 += 50 * (player1 == IJICHI);
                            Score2 += 50 * (player2 == IJICHI);
                            Score3 += 50 * (player3 == IJICHI);
                            Score4 += 50 * (player4 == IJICHI);
                            al_draw_filled_rectangle(j*40-20, i*40-20, j*40+20, i*40+20, YELLOW);
                            break;
                        case YAMADA:
                            Score1 += 50 * (player1 == YAMADA);
                            Score2 += 50 * (player2 == YAMADA);
                            Score3 += 50 * (player3 == YAMADA);
                            Score4 += 50 * (player4 == YAMADA);
                            al_draw_filled_rectangle(j*40-20, i*40-20, j*40+20, i*40+20, BLUE);
                            break;
                        case KITA:
                            Score1 += 50 * (player1 == KITA);
                            Score2 += 50 * (player2 == KITA);
                            Score3 += 50 * (player3 == KITA);
                            Score4 += 50 * (player4 == KITA);
                            al_draw_filled_rectangle(j*40-20, i*40-20, j*40+20, i*40+20, RED);
                            break;
                        default:
                            //std::cout << tmp << ' ' << level->get_character(tmp) << std::endl;
                            al_draw_filled_rectangle(j*40-20, i*40-20, j*40+20, i*40+20, BLACK);
                            break;
                    }
                }
            }
            else if(i >= 6 && i <= 11 && j >= 4 && j <= 12) {
                if (!(level->haveColor(tmp))) {
                    al_draw_filled_rectangle(j*40-20, i*40-20, j*40+20, i*40+20, WHITE);
                }
                else {
                    switch (level->get_character(tmp))
                    {
                        case BOCCHI:
                            Score1 += 200 * (player1 == BOCCHI);
                            Score2 += 200 * (player2 == BOCCHI);
                            Score3 += 200 * (player3 == BOCCHI);
                            Score4 += 200 * (player4 == BOCCHI);
                            al_draw_filled_rectangle(j*40-20, i*40-20, j*40+20, i*40+20, PURPLE);
                            break;
                        case IJICHI:
                            Score1 += 200 * (player1 == IJICHI);
                            Score2 += 200 * (player2 == IJICHI);
                            Score3 += 200 * (player3 == IJICHI);
                            Score4 += 200 * (player4 == IJICHI);
                            al_draw_filled_rectangle(j*40-20, i*40-20, j*40+20, i*40+20, YELLOW);
                            break;
                        case YAMADA:
                            Score1 += 200 * (player1 == YAMADA);
                            Score2 += 200 * (player2 == YAMADA);
                            Score3 += 200 * (player3 == YAMADA);
                            Score4 += 200 * (player4 == YAMADA);
                            al_draw_filled_rectangle(j*40-20, i*40-20, j*40+20, i*40+20, BLUE);
                            break;
                        case KITA:
                            Score1 += 200 * (player1 == KITA);
                            Score2 += 200 * (player2 == KITA);
                            Score3 += 200 * (player3 == KITA);
                            Score4 += 200 * (player4 == KITA);
                            al_draw_filled_rectangle(j*40-20, i*40-20, j*40+20, i*40+20, RED);
                            break;
                        default:
                            al_draw_filled_rectangle(j*40-20, i*40-20, j*40+20, i*40+20, BLACK);
                            break;
                    }
                }
                if (current_wall) al_draw_bitmap(current_wall, j*40-20, i*40-20, 0);
            }
            else if (i >= 5 && i <= 12 && j >= 3 && j <= 13) {
                al_draw_filled_rectangle(j*40-20, i*40-20, j*40+20, i*40+20, WHITE);
                // fprintf(stderr, "DEBUG: Attempting to draw 'wall' at %d,%d.\n", j, i);
                if (current_wall) al_draw_bitmap(current_wall, j*40-20, i*40-20, 0);
                // fprintf(stderr, "DEBUG: OK.\n");
            }

            if (level->have_speed(tmp)) {
                // fprintf(stderr, "DEBUG: Attempting to draw 'speedtool'.\n");
                al_draw_bitmap(speedtool, j*40-15, i*40-15, 0);
                // fprintf(stderr, "DEBUG: OK.\n");
            }
            // al_draw_text(font, al_map_rgb(0, 0, 0), j*40, i*40, ALLEGRO_ALIGN_CENTER, buffer);   // debug
        }
    }
    // renew Score in menu
    menu->Renew_Score1(Score1);
    menu->Renew_Score2(Score2);
    menu->Renew_Score3(Score3);
    menu->Renew_Score4(Score4);

    // draw bandmember
    // fprintf(stderr, "DEBUG: Drawing game objects now...\n");
    for (int i = BOCCHI; i <= KITA; i++) 
    {
        // fprintf(stderr, "DEBUG: Attempting to call Draw() for BandMember %d.\n", i);
        bandmemberSet[i]->Draw();
        // fprintf(stderr, "DEBUG: OK.\n");
    }

    // draw bomb && fire
    std::vector<Bomb*>::iterator it;
    for(it=bombSet.begin(); it!=bombSet.end();)          
    {
        // fprintf(stderr, "DEBUG: Processing a bomb object.\n");
        if ((*it)->get_counter() <= 135) {
            // fprintf(stderr, "DEBUG: Attempting to call Draw() for a Bomb.\n");
            (*it)->Draw();
            // fprintf(stderr, "DEBUG: OK.\n");
            it++;
        }
        else if ((*it)->get_counter() <= 190) {
            bool DIR[5];
            int now_x = (*it)->getX(), now_y = (*it)->getY();
            int next_x, next_y;
            int idx;
            for (unsigned dir = LEFT; dir <= DOWN; dir++) {
                next_x = now_x + axis_x[dir]*grid_width;
                next_y = now_y + axis_y[dir]*grid_height;
                idx = (next_y * 15 + next_x)/40;
                level->bomb_yes(idx);
                if (level->isRoad(idx) || level->isStone(idx))
                    DIR[dir] = true;
                else
                    DIR[dir] = false;
            }
            // fprintf(stderr, "DEBUG: Attempting to call DrawFire() for a Bomb.\n");
            (*it)->DrawFire(DIR[LEFT], DIR[RIGHT], DIR[UP], DIR[DOWN]);
            // fprintf(stderr, "DEBUG: OK.\n");
            it++;
        }
        else {
            int idx_cur = ((*it)->getY() * 15 + (*it)->getX())/40;
            int idx_left = idx_cur - 1;
            int idx_right = idx_cur + 1;
            int idx_down = idx_cur - 15;
            int idx_up = idx_cur + 15;
            if(level->isStone(idx_left)) {
                level->stone_bomb(idx_left);
                level->speed_emerge(idx_left);
            }
            if(level->isStone(idx_right)) {
                level->stone_bomb(idx_right);
                level->speed_emerge(idx_right);
            }
            if(level->isStone(idx_down)) {
                level->stone_bomb(idx_down);
                level->speed_emerge(idx_down);
            }
            if(level->isStone(idx_up)) {
                level->stone_bomb(idx_up);
                level->speed_emerge(idx_up);
            }
            
            level->change_character(idx_cur, (*it)->get_character());
            level->change_character(idx_left, (*it)->get_character());
            level->change_character(idx_right, (*it)->get_character());
            level->change_character(idx_down, (*it)->get_character());
            level->change_character(idx_up, (*it)->get_character());

            level->bomb_not(idx_left);
            level->bomb_not(idx_right);
            level->bomb_not(idx_down);
            level->bomb_not(idx_up);
            
        
            bombSet.erase(it);
        }
    }

    al_draw_filled_rectangle(field_width, 0, window_width, window_height, al_map_rgb(100, 100, 100));
    
    // fprintf(stderr, "DEBUG: Attempting to call Draw() for Menu (HUD).\n");
    menu->Draw();
    // fprintf(stderr, "DEBUG: OK.\n");

    al_flip_display();
    // fprintf(stderr, "DEBUG: draw_running_map finished one frame successfully.\n");

}

void
GameWindow::draw_menu() {

    al_clear_to_color(al_map_rgb(100, 100, 100));
    al_draw_bitmap(background_menu, 0, 0, 0);

    al_flip_display();
}

void
GameWindow::draw_charchoose() {

    al_clear_to_color(al_map_rgb(100, 100, 100));
    al_draw_bitmap(background_charchoose, 0, 0, 0);

    al_flip_display();
}

void
GameWindow::draw_charstory() {

    al_clear_to_color(al_map_rgb(100, 100, 100));
    al_draw_bitmap(background_charstory, 0, 0, 0);

    al_flip_display();
}

void
GameWindow::draw_introduce() {

    al_clear_to_color(al_map_rgb(100, 100, 100));
    al_draw_bitmap(background_introduce, 0, 0, 0);

    al_flip_display();
}

void
GameWindow::draw_P1_P2() {

    al_clear_to_color(al_map_rgb(100, 100, 100));
    al_draw_bitmap(background_P1_P2, 0, 0, 0);

    al_flip_display();
}

void
GameWindow::draw_P2_P1() {

    al_clear_to_color(al_map_rgb(100, 100, 100));
    al_draw_bitmap(background_P2_P1, 0, 0, 0);

    al_flip_display();
}

void
GameWindow::draw_P2_P1bocchi() {

    al_clear_to_color(al_map_rgb(100, 100, 100));
    al_draw_bitmap(background_P2_P1bocchi, 0, 0, 0);

    al_flip_display();
}

void
GameWindow::draw_P2_P1yamada() {

    al_clear_to_color(al_map_rgb(100, 100, 100));
    al_draw_bitmap(background_P2_P1yamada, 0, 0, 0);

    al_flip_display();
}

void
GameWindow::draw_P2_P1ijichi() {

    al_clear_to_color(al_map_rgb(100, 100, 100));
    al_draw_bitmap(background_P2_P1ijichi, 0, 0, 0);

    al_flip_display();
}

void
GameWindow::draw_P2_P1kita() {

    al_clear_to_color(al_map_rgb(100, 100, 100));
    al_draw_bitmap(background_P2_P1kita, 0, 0, 0);

    al_flip_display();
}

void
GameWindow::draw_mapchoose() {

    al_clear_to_color(al_map_rgb(100, 100, 100));
    al_draw_bitmap(background_mapchoose, 0, 0, 0);

    al_flip_display();
}

void
GameWindow::draw_pause() {

    al_clear_to_color(al_map_rgb(100, 100, 100));
    al_draw_bitmap(background_pause, 0, 0, 0);

    // printf("PASS\n");y
    al_flip_display();
}

void
GameWindow::draw_end() {
    if (menu->win()) {
        al_clear_to_color(al_map_rgb(100, 100, 100));
        al_draw_bitmap(background_win, 0, 0, 0);
    }
    else {
        al_clear_to_color(al_map_rgb(100, 100, 100));
        al_draw_bitmap(background_lose, 0, 0, 0);
    }
    al_flip_display();
}
