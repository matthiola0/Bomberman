#include "GameWindow.h"
#include "global.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <queue>
#include <algorithm>
#include <vector>


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
        // 憒?頛憭望?嚗?仿＊蝷粹隤方??臭蒂蝯迫?
        fprintf(stderr, "Fatal Error: Failed to load bitmap: %s\n", path);
        show_err_msg(GAME_TERMINATE);
    }
    return bmp;
}

ALLEGRO_FONT* GameWindow::load_font_safely(const char* path, int size, int flags)
{
    ALLEGRO_FONT* font = al_load_ttf_font(path, size, flags);
    if (!font) {
        // 憒?頛憭望?嚗?仿＊蝷粹隤方??臭蒂蝯迫?
        fprintf(stderr, "Fatal Error: Failed to load font: %s\n", path);
        show_err_msg(GAME_TERMINATE);
    }
    return font;
}

void
GameWindow::game_init()
{
    // char buffer[50];

    icon = load_bitmap_safely("src/icon.png");
    background1 = load_bitmap_safely("src/Asset/school.jpg");
    background2 = load_bitmap_safely("src/Asset/home.jpg");
    background3 = load_bitmap_safely("src/Asset/starry.png");
    background_menu = load_bitmap_safely("src/Asset/menu.png");
    background_charchoose = load_bitmap_safely("src/Asset/characterchoose.png");
    background_charstory = load_bitmap_safely("src/Asset/characterstory.png");
    background_introduce = load_bitmap_safely("src/Asset/introduce.png");
    background_pause = load_bitmap_safely("src/Asset/pause.png");
    background_win = load_bitmap_safely("src/Asset/win.png");
    background_lose = load_bitmap_safely("src/Asset/lose.png");
    background_P1_P2 = load_bitmap_safely("src/Asset/P1_P2.png");
    background_P2_P1 = load_bitmap_safely("src/Asset/P2_P1.png");
    background_P2_P1bocchi = load_bitmap_safely("src/Asset/P2_P1bocchi.png");
    background_P2_P1ijichi = load_bitmap_safely("src/Asset/P2_P1ijichi.png");
    background_P2_P1kita = load_bitmap_safely("src/Asset/P2_P1kita.png");
    background_P2_P1yamada = load_bitmap_safely("src/Asset/P2_P1yamada.png");
    background_mapchoose = load_bitmap_safely("src/Asset/mapchoose.png");
    speedtool = load_bitmap_safely("src/Asset/speedtool.png");

    // *** 撠???隞嗡?甈⊥扯???***
    wall_school = load_bitmap_safely("src/Asset/table.png");
    stone_school = load_bitmap_safely("src/Asset/chair.png");
    wall_home = load_bitmap_safely("src/Asset/homewall.png");
    stone_home = load_bitmap_safely("src/Asset/homestone.png");
    wall_starry = load_bitmap_safely("src/Asset/starrywall.png");
    stone_starry = load_bitmap_safely("src/Asset/starrystone.png");

    // wall = al_load_bitmap("./Asset/table.png");
    // stone = al_load_bitmap("./Asset/chair.png");

    al_set_display_icon(display, icon);
    al_reserve_samples(3);


    sample = al_load_sample("src/bocchi_song.mp3");
    if (!sample) {
        fprintf(stderr, "Fatal Error: Failed to load audio sample: src/bocchi_song.mp3\n");
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

    for (int i = 0; i < ALLEGRO_KEY_MAX; i++) key[i] = false;
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

    al_init_image_addon(); // initialize the image addon
    al_init_font_addon(); // initialize the font addon
    al_init_ttf_addon(); // initialize the ttf (True Type Font) addon
    al_init_primitives_addon();

    al_install_audio();    // install audio event
    al_init_acodec_addon(); // initialize acodec addon

    al_install_keyboard(); // install keyboard event
    al_install_mouse();    // install mouse event

    font = load_font_safely("src/Caviar_Dreams_Bold.ttf", 12, 0);
    Medium_font = load_font_safely("src/Caviar_Dreams_Bold.ttf", 24, 0);
    Large_font = load_font_safely("src/Caviar_Dreams_Bold.ttf", 36, 0);

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
    int lw = level->getWidth();
    if (menu->if_gaame_started())
    {
        // Update Player 1 movement flags based on keys
        if (key[ALLEGRO_KEY_W]) { bandmemberSet[player1]->changeDir(UP); moving1 = true; }
        else if (key[ALLEGRO_KEY_S]) { bandmemberSet[player1]->changeDir(DOWN); moving1 = true; }
        else if (key[ALLEGRO_KEY_A]) { bandmemberSet[player1]->changeDir(LEFT); moving1 = true; }
        else if (key[ALLEGRO_KEY_D]) { bandmemberSet[player1]->changeDir(RIGHT); moving1 = true; }
        else { moving1 = false; }

        // Update Player 2 movement flags based on keys
        if (two_player) {
            if (key[ALLEGRO_KEY_UP]) { bandmemberSet[player2]->changeDir(UP); moving2 = true; }
            else if (key[ALLEGRO_KEY_DOWN]) { bandmemberSet[player2]->changeDir(DOWN); moving2 = true; }
            else if (key[ALLEGRO_KEY_LEFT]) { bandmemberSet[player2]->changeDir(LEFT); moving2 = true; }
            else if (key[ALLEGRO_KEY_RIGHT]) { bandmemberSet[player2]->changeDir(RIGHT); moving2 = true; }
            else { moving2 = false; }
        } else {
            update_ai(player2, moving2);
        }

        update_ai(player3, moving3);
        update_smart_ai(player4, moving4);


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

    // player1
    if (moving1 && can_move(player1, bandmemberSet[player1]->getDir())) {
        bandmemberSet[player1]->Move();
        
        // Speed item check
        int x = bandmemberSet[player1]->getX();
        int y = bandmemberSet[player1]->getY();
        int idx = (y / grid_height) * lw + (x / grid_width);
        if (level->have_speed(idx)) {
            level->speed_disappear(idx);
            bandmemberSet[player1]->SpeedChange(2);
        }
    }
    // Bombing check
    int idx1 = (bandmemberSet[player1]->getY() / grid_height) * lw + (bandmemberSet[player1]->getX() / grid_width);
    if (level->is_bombing(idx1)) level->clear_color(player1);

    // player2
    if (moving2 && can_move(player2, bandmemberSet[player2]->getDir())) {
        bandmemberSet[player2]->Move();
        
        int x = bandmemberSet[player2]->getX();
        int y = bandmemberSet[player2]->getY();
        int idx = (y / grid_height) * lw + (x / grid_width);
        if (level->have_speed(idx)) {
            level->speed_disappear(idx);
            bandmemberSet[player2]->SpeedChange(2);
        }
    }
    int idx2 = (bandmemberSet[player2]->getY() / grid_height) * lw + (bandmemberSet[player2]->getX() / grid_width);
    if (level->is_bombing(idx2)) {
        if (two_player) level->clear_color(player2);
    }

    // player3 (AI)
    if (moving3 && can_move(player3, bandmemberSet[player3]->getDir())) {
        bandmemberSet[player3]->Move();
        
        int x = bandmemberSet[player3]->getX();
        int y = bandmemberSet[player3]->getY();
        int idx = (y / grid_height) * lw + (x / grid_width);
        if (level->have_speed(idx)) {
            level->speed_disappear(idx);
            bandmemberSet[player3]->SpeedChange(2);
        }
    }

    // player4 (AI)
    if (moving4 && can_move(player4, bandmemberSet[player4]->getDir())) {
        bandmemberSet[player4]->Move();
        
        int x = bandmemberSet[player4]->getX();
        int y = bandmemberSet[player4]->getY();
        int idx = (y / grid_height) * lw + (x / grid_width);
        if (level->have_speed(idx)) {
            level->speed_disappear(idx);
            bandmemberSet[player4]->SpeedChange(2);
        }
    }
    

    return GAME_CONTINUE;
}

void
GameWindow::game_reset()
{
    for (auto b : bandmemberSet) {
        delete b;
    }
    bandmemberSet.clear();

    for (auto b : bombSet) {
        delete b;
    }
    bombSet.clear();

    moving1 = false;
    moving2 = false;
    moving3 = false;
    moving4 = false;

    Time_Inc_Count = 0;
    // Monster_Pro_Count = 0;
    mute = false;
    redraw = false;
    menu->Reset();

    // Re-populate bandmemberSet
    for (int i = BOCCHI; i <= KITA; i++) {
        bandmemberSet.push_back(new BandMember(i));
    }

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

void GameWindow::enter_game(int map_id) {
    game_reset();
    level->setLevel(map_id);
    
    // Reset players again after setLevel potentially cleared something or changed layout
    int lw = level->getWidth();
    for (int it = 0; it < (int)bandmemberSet.size(); it++) {
        if (bandmemberSet[it]) bandmemberSet[it]->Reset(lw);
    }
    
    moving1 = moving2 = moving3 = moving4 = false;
    
    if (map_id == 1) scene = GAMESCHOOL;
    else if (map_id == 2) scene = GAMEHOME;
    else if (map_id == 3) scene = GAMESTARRY;
    
    menu->set_game_started(true);
    // Background sound and timer moved from MAPCHOOSE to match start
    if (!mute) al_play_sample_instance(backgroundSound);
    al_start_timer(timer);
}

bool GameWindow::can_move(int player_idx, int dir) {
    if (bandmemberSet.empty() || player_idx < 0 || player_idx >= (int)bandmemberSet.size()) return false;
    if (!bandmemberSet[player_idx]) return false;

    int x_now = bandmemberSet[player_idx]->getX();
    int y_now = bandmemberSet[player_idx]->getY();
    int speed = 3; // Use a slightly larger step for better collision detection
    int x_next = x_now + speed * axis_x[dir];
    int y_next = y_now + speed * axis_y[dir];

    // Check corners of the member for collision with a small padding for smoother movement
    int lw = level->getWidth();
    int padding = 6;
    int x_left = x_next - member_width / 2 + padding;
    int x_right = x_next + member_width / 2 - 1 - padding;
    int y_top = y_next - member_width / 2 + padding;
    int y_bottom = y_next + member_width / 2 - 1 - padding;

    int t1 = (x_left / grid_width + (y_top / grid_height) * lw);
    int t2 = (x_left / grid_width + (y_bottom / grid_height) * lw);
    int t3 = (x_right / grid_width + (y_top / grid_height) * lw);
    int t4 = (x_right / grid_width + (y_bottom / grid_height) * lw);

    return (level->isRoad(t1) && level->isRoad(t2) && level->isRoad(t3) && level->isRoad(t4));
}

void GameWindow::update_ai(int player_idx, bool &moving_flag) {
    if (bandmemberSet.empty() || player_idx < 0 || player_idx >= (int)bandmemberSet.size()) return;

    if (!bandmemberSet[player_idx]) return;

    int current_dir = bandmemberSet[player_idx]->getDir();
    
    // 1. Check for nearby danger (bombs about to explode)
    int ai_x = bandmemberSet[player_idx]->getX();
    int ai_y = bandmemberSet[player_idx]->getY();
    int danger_dir = -1;
    float min_dist = 999999;

    for (auto b : bombSet) {
        if (b && b->get_counter() > 60) { // Bomb is dangerous after 1 second
            float dx = b->getX() - ai_x;
            float dy = b->getY() - ai_y;
            float d = dx*dx + dy*dy;
            if (d < 3 * grid_width * grid_width) { // roughly within 1-2 cells
                if (d < min_dist) {
                    min_dist = d;
                    // Simplistic escape: move in opposite direction
                    if (abs(dx) > abs(dy)) danger_dir = (dx > 0) ? LEFT : RIGHT;
                    else danger_dir = (dy > 0) ? UP : DOWN;
                }
            }
        }
    }

    // 2. Decide movement
    bool force_change = !can_move(player_idx, current_dir);
    
    if (danger_dir != -1 && can_move(player_idx, danger_dir)) {
        bandmemberSet[player_idx]->changeDir(danger_dir);
        moving_flag = true;
    }
    else if (force_change || (rand() % 20 == 0)) { // More frequent decisions (1/20 instead of 1/60)
        std::vector<int> valid_dirs;
        for (int d = LEFT; d <= DOWN; d++) {
            if (can_move(player_idx, d)) {
                valid_dirs.push_back(d);
            }
        }
        
        if (!valid_dirs.empty()) {
            int new_dir = valid_dirs[rand() % valid_dirs.size()];
            bandmemberSet[player_idx]->changeDir(new_dir);
            moving_flag = true;
        } else {
            moving_flag = false;
        }
    } else {
        // If not blocked and not making a decision, keep moving
        moving_flag = true;
    }

    // 3. Bomb placement logic
    // AI has a chance to drop a bomb if near a stone
    if (rand() % 120 == 0) {
        int x = bandmemberSet[player_idx]->getX();
        int y = bandmemberSet[player_idx]->getY();
        int grid_x = (x + grid_width / 2) / grid_width;
        int grid_y = (y + grid_height / 2) / grid_height;
        int idx = grid_y * level->getWidth() + grid_x;

        // Check adjacent cells for stones
        bool stone_nearby = false;
        int adj[] = {idx - 1, idx + 1, idx - level->getWidth(), idx + level->getWidth()};
        for (int i = 0; i < 4; i++) {
            if (level->isStone(adj[i])) {
                stone_nearby = true;
                break;
            }
        }

        if (stone_nearby) {
            // Check if there's already a bomb at this location
            bool already_bombed = false;
            int gx = (x + grid_width / 2) / grid_width * grid_width;
            int gy = (y + grid_height / 2) / grid_height * grid_height;
            for (auto b : bombSet) {
                if (b->getX() == gx && b->getY() == gy) {
                    already_bombed = true;
                    break;
                }
            }
            if (!already_bombed) {
                Bomb* bomb = new Bomb(x, y, BandMemberClass[player_idx]);
                bombSet.push_back(bomb);
            }
        }
    }
}

int
GameWindow::process_event()
{
    int i;
    int instruction = GAME_CONTINUE;

    al_wait_for_event(event_queue, &event);
    redraw = false;

    if(event.type == ALLEGRO_EVENT_TIMER) {
        if(event.timer.source == timer) {
            redraw = true;

            // if (menu->if_gaame_started())
            // {
            //     // fprintf(stderr, "DEBUG: Game has started, updating time. Current Time = %d\n", menu->getTime());

            //     int randomNum = rand() % 320;
            //     if (randomNum == 0) {
            //         bandmemberSet[player3]->changeDir(2); // move up
            //         moving3 = true;
            //     } else if (randomNum == 1) {
            //         bandmemberSet[player3]->changeDir(3); // move down
            //         moving3 = true;
            //     } else if (randomNum == 2) {
            //         bandmemberSet[player3]->changeDir(0); // move left
            //         moving3 = true;
            //     } else if (randomNum == 3) {
            //         bandmemberSet[player3]->changeDir(1); // move right
            //     }
            //     else if (randomNum == 4) {
            //         Bomb* bomb = new Bomb(bandmemberSet[player3]->getX(), bandmemberSet[player3]->getY(), BandMemberClass[player3]);
            //         bombSet.push_back(bomb);
            //     }

            //     randomNum = rand() % 320;
            //     if (randomNum == 0) {
            //         bandmemberSet[player4]->changeDir(2); // move up
            //         moving4 = true;
            //     } else if (randomNum == 1) {
            //         bandmemberSet[player4]->changeDir(3); // move down
            //         moving4 = true;
            //     } else if (randomNum == 2) {
            //         bandmemberSet[player4]->changeDir(0); // move left
            //         moving4 = true;
            //     } else if (randomNum == 3) {
            //         bandmemberSet[player4]->changeDir(1); // move right
            //     }
            //     else if (randomNum == 4) {
            //         Bomb* bomb = new Bomb(bandmemberSet[player4]->getX(), bandmemberSet[player4]->getY(), BandMemberClass[player4]);
            //         bombSet.push_back(bomb);
            //     }

            //     if (!two_player) {
            //         randomNum = rand() % 320;
            //         if (randomNum == 0) {
            //             bandmemberSet[player2]->changeDir(2); // move up
            //             moving2 = true;
            //         } else if (randomNum == 1) {
            //             bandmemberSet[player2]->changeDir(3); // move down
            //             moving2 = true;
            //         } else if (randomNum == 2) {
            //             bandmemberSet[player2]->changeDir(0); // move left
            //             moving2 = true;
            //         } else if (randomNum == 3) {
            //             bandmemberSet[player2]->changeDir(1); // move right
            //         }
            //         else if (randomNum == 4) {
            //             Bomb* bomb = new Bomb(bandmemberSet[player2]->getX(), bandmemberSet[player2]->getY(), BandMemberClass[player2]);
            //             bombSet.push_back(bomb);
            //         }
            //     }

            //     menu->Change_Time();
            // }

            // for (int i = BOCCHI; i <= KITA; i++) {
            //     bandmemberSet[i]->change_counter();

            //     if (bandmemberSet[i]->getSCounter() <= 0)
            //         bandmemberSet[i]->SpeedChange(1);
            // }

            // for (int i=0; i<bombSet.size(); i++)
            // {
            //     bombSet[i]->change_counter();
            // }
        }
    }
    else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        return GAME_EXIT;
    }
    else if(event.type == ALLEGRO_EVENT_KEY_DOWN && scene >= GAMESCHOOL && scene <= GAMESTARRY) {
        key[event.keyboard.keycode] = true;
        Bomb *bomb;
        switch(event.keyboard.keycode) {   
            case ALLEGRO_KEY_P:
                if(al_get_timer_started(timer)) {
                    al_stop_timer(timer);
                    cur_scene = scene;
                    scene = GAMEPAUSE;
                    redraw = true;
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
            case ALLEGRO_KEY_SPACE:
                bomb = new Bomb(bandmemberSet[player1]->getX(), bandmemberSet[player1]->getY(), BandMemberClass[player1]);
                bombSet.push_back(bomb);
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
        key[event.keyboard.keycode] = false;
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
                        enter_game(1);
                    }
                    else if(mouse_hover(0,200,800,200)) {
                        enter_game(2);
                    }
                    else if(mouse_hover(0,400,800,200)) {
                        enter_game(3);
                    }
                    break;
                case GAMEEND:
                    if (mouse_hover(0, 540,  188-0, 580-540)) {
                        scene = MENU;
                        menu->set_game_started(false);
                    }
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
        // instruction = game_update();

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
                instruction = game_update();
                draw_running_map(GAMESCHOOL);
                break;
            case GAMEHOME:
                if (menu->getTime() <= 0) {
                    scene = GAMEEND; 
                }
                instruction = game_update();
                draw_running_map(GAMEHOME);
                break;
            case GAMESTARRY:
                if (menu->getTime() <= 0) {
                    scene = GAMEEND; 
                }
                instruction = game_update();
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

    // Update camera position
    if (bandmemberSet.empty() || player1 < 0 || player1 >= (int)bandmemberSet.size() || !bandmemberSet[player1]) return;

    int px = bandmemberSet[player1]->getX();
    int py = bandmemberSet[player1]->getY();
    int lw = level->getWidth();
    int lh = level->getHeight();
    int map_w = lw * grid_width;
    int map_h = lh * grid_height;

    cam_x = px - field_width / 2;
    cam_y = py - field_height / 2;

    if (cam_x < 0) cam_x = 0;
    if (cam_y < 0) cam_y = 0;
    if (cam_x > map_w - field_width) cam_x = map_w - field_width;
    if (cam_y > map_h - field_height) cam_y = map_h - field_height;
    
    // For small maps (L1/L2), center the map or keep layout fixed
    if (map_w <= field_width) cam_x = (map_w - field_width) / 2;
    if (map_h <= field_height) cam_y = (map_h - field_height) / 2;

    ALLEGRO_TRANSFORM trans;
    al_identity_transform(&trans);
    al_translate_transform(&trans, -cam_x, -cam_y);
    al_use_transform(&trans);

    switch (scene) {
        case GAMESCHOOL:
            al_draw_bitmap(background1, 0, 0, 0);
            current_wall = wall_school;
            current_stone = stone_school;
            break;
        case GAMEHOME:
            al_draw_bitmap(background2, 0, 0, 0);
            current_wall = wall_home;
            current_stone = stone_home;
            break;
        case GAMESTARRY:
            al_draw_bitmap(background3, 0, 0, 0);
            current_wall = wall_starry;
            current_stone = stone_starry;
            break;
    }

    int Score1 = 0, Score2 = 0, Score3 = 0, Score4 = 0;

    // draw map
    for(i = 0; i < (unsigned int)lh; i++)
    {
        for(j = 0; j < (unsigned int)lw; j++)
        {
            int tmp = i * lw + j;
            if(level->isStone(tmp)) {
                if (current_stone) al_draw_bitmap(current_stone, j*40, i*40, 0);
            }
            else if(level->isRoad(tmp)) {
                if (!(level->haveColor(tmp))) {
                    al_draw_filled_rectangle(j*40, i*40, j*40+40, i*40+40, WHITE);
                }
                else {
                    switch (level->get_character(tmp))
                    {
                    case BOCCHI:
                        al_draw_filled_rectangle(j*40, i*40, j*40+40, i*40+40, PURPLE);
                        Score1 += 50 * (player1 == BOCCHI);
                        Score2 += 50 * (player2 == BOCCHI);
                        Score3 += 50 * (player3 == BOCCHI);
                        Score4 += 50 * (player4 == BOCCHI);
                        break;
                    case IJICHI:
                        al_draw_filled_rectangle(j*40, i*40, j*40+40, i*40+40, YELLOW);
                        Score1 += 50 * (player1 == IJICHI);
                        Score2 += 50 * (player2 == IJICHI);
                        Score3 += 50 * (player3 == IJICHI);
                        Score4 += 50 * (player4 == IJICHI);
                        break;
                    case YAMADA:
                        al_draw_filled_rectangle(j*40, i*40, j*40+40, i*40+40, BLUE);
                        Score1 += 50 * (player1 == YAMADA);
                        Score2 += 50 * (player2 == YAMADA);
                        Score3 += 50 * (player3 == YAMADA);
                        Score4 += 50 * (player4 == YAMADA);
                        break;
                    case KITA:
                        al_draw_filled_rectangle(j*40, i*40, j*40+40, i*40+40, RED);
                        Score1 += 50 * (player1 == KITA);
                        Score2 += 50 * (player2 == KITA);
                        Score3 += 50 * (player3 == KITA);
                        Score4 += 50 * (player4 == KITA);
                        break;
                    }
                }
            }
            else {
                // Any tile that is NOT road and NOT stone is an indestructible WALL
                al_draw_filled_rectangle(j*40, i*40, j*40+40, i*40+40, WHITE);
                if (current_wall) al_draw_bitmap(current_wall, j*40, i*40, 0);
            }

            if (level->have_speed(tmp)) {
                al_draw_bitmap(speedtool, j*40+5, i*40+5, 0);
            }
        }
    }
    
    // renew Score in menu
    menu->Renew_Score1(Score1);
    menu->Renew_Score2(Score2);
    menu->Renew_Score3(Score3);
    menu->Renew_Score4(Score4);

    // draw bandmember
    for (int k = 0; k < (int)bandmemberSet.size(); k++) 
    {
        if (bandmemberSet[k]) bandmemberSet[k]->Draw();
    }

    // draw bomb && fire
    std::vector<Bomb*>::iterator bit;
    for(bit=bombSet.begin(); bit!=bombSet.end();)          
    {
        if ((*bit)->get_counter() <= 135) {
            (*bit)->Draw();
            bit++;
        }
        else if ((*bit)->get_counter() <= 190) {
            bool DIR[5];
            int now_x = (*bit)->getX(), now_y = (*bit)->getY();
            int next_x, next_y;
            int idx;
            for (unsigned dir = LEFT; dir <= DOWN; dir++) {
                next_x = now_x + axis_x[dir]*grid_width;
                next_y = now_y + axis_y[dir]*grid_height;
                idx = (next_y / grid_height * lw) + (next_x / grid_width);
                level->bomb_yes(idx);
                if (level->isRoad(idx) || level->isStone(idx))
                    DIR[dir] = true;
                else
                    DIR[dir] = false;
            }
            (*bit)->DrawFire(DIR[LEFT], DIR[RIGHT], DIR[UP], DIR[DOWN]);
            bit++;
        }
        else {
            int idx_cur = ((*bit)->getY() / grid_height * lw) + ((*bit)->getX() / grid_width);
            int idx_left = idx_cur - 1;
            int idx_right = idx_cur + 1;
            int idx_up = idx_cur - lw;
            int idx_down = idx_cur + lw;
            
            if(level->isStone(idx_left)) { level->stone_bomb(idx_left); level->speed_emerge(idx_left); }
            if(level->isStone(idx_right)) { level->stone_bomb(idx_right); level->speed_emerge(idx_right); }
            if(level->isStone(idx_down)) { level->stone_bomb(idx_down); level->speed_emerge(idx_down); }
            if(level->isStone(idx_up)) { level->stone_bomb(idx_up); level->speed_emerge(idx_up); }
            
            level->change_character(idx_cur, (*bit)->get_character());
            level->change_character(idx_left, (*bit)->get_character());
            level->change_character(idx_right, (*bit)->get_character());
            level->change_character(idx_down, (*bit)->get_character());
            level->change_character(idx_up, (*bit)->get_character());

            level->bomb_not(idx_left);
            level->bomb_not(idx_right);
            level->bomb_not(idx_down);
            level->bomb_not(idx_up);
            
            delete (*bit);
            bit = bombSet.erase(bit);
        }
    }

    al_identity_transform(&trans);
    al_use_transform(&trans);

    al_draw_filled_rectangle(field_width, 0, window_width, window_height, al_map_rgb(100, 100, 100));
    menu->Draw();

    al_flip_display();
}

void GameWindow::draw_menu() {
    al_clear_to_color(al_map_rgb(100, 100, 100));
    al_draw_bitmap(background_menu, 0, 0, 0);
    al_flip_display();
}

void GameWindow::draw_charchoose() {
    al_clear_to_color(al_map_rgb(100, 100, 100));
    al_draw_bitmap(background_charchoose, 0, 0, 0);
    al_flip_display();
}

void GameWindow::draw_charstory() {
    al_clear_to_color(al_map_rgb(100, 100, 100));
    al_draw_bitmap(background_charstory, 0, 0, 0);
    al_flip_display();
}

void GameWindow::draw_introduce() {
    al_clear_to_color(al_map_rgb(100, 100, 100));
    al_draw_bitmap(background_introduce, 0, 0, 0);
    al_flip_display();
}

void GameWindow::draw_P1_P2() {
    al_clear_to_color(al_map_rgb(100, 100, 100));
    al_draw_bitmap(background_P1_P2, 0, 0, 0);
    al_flip_display();
}

void GameWindow::draw_P2_P1() {
    al_clear_to_color(al_map_rgb(100, 100, 100));
    al_draw_bitmap(background_P2_P1, 0, 0, 0);
    al_flip_display();
}

void GameWindow::draw_P2_P1bocchi() {
    al_clear_to_color(al_map_rgb(100, 100, 100));
    al_draw_bitmap(background_P2_P1bocchi, 0, 0, 0);
    al_flip_display();
}

void GameWindow::draw_P2_P1yamada() {
    al_clear_to_color(al_map_rgb(100, 100, 100));
    al_draw_bitmap(background_P2_P1yamada, 0, 0, 0);
    al_flip_display();
}

void GameWindow::draw_P2_P1ijichi() {
    al_clear_to_color(al_map_rgb(100, 100, 100));
    al_draw_bitmap(background_P2_P1ijichi, 0, 0, 0);
    al_flip_display();
}

void GameWindow::draw_P2_P1kita() {
    al_clear_to_color(al_map_rgb(100, 100, 100));
    al_draw_bitmap(background_P2_P1kita, 0, 0, 0);
    al_flip_display();
}

void GameWindow::draw_mapchoose() {
    al_clear_to_color(al_map_rgb(100, 100, 100));
    al_draw_bitmap(background_mapchoose, 0, 0, 0);
    al_flip_display();
}

void GameWindow::draw_pause() {
    al_clear_to_color(al_map_rgb(100, 100, 100));
    al_draw_bitmap(background_pause, 0, 0, 0);
    al_flip_display();
}

void GameWindow::draw_end() {
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



void GameWindow::update_smart_ai(int player_idx, bool &moving_flag) {
    if (bandmemberSet.empty() || player_idx < 0 || player_idx >= (int)bandmemberSet.size()) return;
    if (!bandmemberSet[player_idx]) return;

    int ai_x = bandmemberSet[player_idx]->getX();
    int ai_y = bandmemberSet[player_idx]->getY();
    int grid_x = (ai_x + grid_width / 2) / grid_width;
    int grid_y = (ai_y + grid_height / 2) / grid_height;
    int lw = level->getWidth();
    int lh = level->getHeight();
    int ai_idx = grid_y * lw + grid_x;

    // 1. Danger avoidance (Highest Priority)
    int danger_dir = -1;
    float min_dist_sq = 999999.0f;
    for (auto b : bombSet) {
        if (b && b->get_counter() > 40) { // Imminent danger
            float dx = (float)(b->getX() - ai_x);
            float dy = (float)(b->getY() - ai_y);
            float d2 = dx*dx + dy*dy;
            if (d2 < 2.5 * grid_width * grid_width) {
                if (d2 < min_dist_sq) {
                    min_dist_sq = d2;
                    if (std::abs(dx) > std::abs(dy)) danger_dir = (dx > 0) ? LEFT : RIGHT;
                    else danger_dir = (dy > 0) ? UP : DOWN;
                }
            }
        }
    }

    if (danger_dir != -1 && can_move(player_idx, danger_dir)) {
