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
void set_attack_volume(float v) { Attack::volume = v; }

ALLEGRO_BITMAP* GameWindow::load_bitmap_safely(const char* p) {
    ALLEGRO_BITMAP* b = al_load_bitmap(p);
    if (!b) { fprintf(stderr, "Err: %s\n", p); show_err_msg(GAME_TERMINATE); }
    return b;
}

ALLEGRO_FONT* GameWindow::load_font_safely(const char* p, int s, int f) {
    ALLEGRO_FONT* fo = al_load_ttf_font(p, s, f);
    if (!fo) { fprintf(stderr, "Err: %s\n", p); show_err_msg(GAME_TERMINATE); }
    return fo;
}

void GameWindow::game_init() {
    icon=load_bitmap_safely("src/icon.png"); background1=load_bitmap_safely("src/Asset/school.jpg"); background2=load_bitmap_safely("src/Asset/home.jpg"); background3=load_bitmap_safely("src/Asset/starry.png"); background_menu=load_bitmap_safely("src/Asset/menu.png"); background_charchoose=load_bitmap_safely("src/Asset/characterchoose.png"); background_charstory=load_bitmap_safely("src/Asset/characterstory.png"); background_introduce=load_bitmap_safely("src/Asset/introduce.png"); background_pause=load_bitmap_safely("src/Asset/pause.png"); background_win=load_bitmap_safely("src/Asset/win.png"); background_lose=load_bitmap_safely("src/Asset/lose.png"); background_P1_P2=load_bitmap_safely("src/Asset/P1_P2.png"); background_P2_P1=load_bitmap_safely("src/Asset/P2_P1.png"); background_P2_P1bocchi=load_bitmap_safely("src/Asset/P2_P1bocchi.png"); background_P2_P1ijichi=load_bitmap_safely("src/Asset/P2_P1ijichi.png"); background_P2_P1kita=load_bitmap_safely("src/Asset/P2_P1kita.png"); background_P2_P1yamada=load_bitmap_safely("src/Asset/P2_P1yamada.png"); background_mapchoose=load_bitmap_safely("src/Asset/mapchoose.png"); speedtool=load_bitmap_safely("src/Asset/speedtool.png"); wall_school=load_bitmap_safely("src/Asset/table.png"); stone_school=load_bitmap_safely("src/Asset/chair.png"); wall_home=load_bitmap_safely("src/Asset/homewall.png"); stone_home=load_bitmap_safely("src/Asset/homestone.png"); wall_starry=load_bitmap_safely("src/Asset/starrywall.png"); stone_starry=load_bitmap_safely("src/Asset/starrystone.png"); al_set_display_icon(display, icon); al_reserve_samples(3); sample=al_load_sample("src/bocchi_song.mp3"); if(!sample){show_err_msg(-1);} backgroundSound=al_create_sample_instance(sample); al_set_sample_instance_playmode(backgroundSound,ALLEGRO_PLAYMODE_LOOP); al_attach_sample_instance_to_mixer(backgroundSound,al_get_default_mixer()); bandmemberSet.clear(); bombSet.clear(); player1=BOCCHI; player2=IJICHI; player3=YAMADA; player4=KITA; two_player=true; level=new LEVEL(1); menu=new Menu(); scene=MENU; bandmemberSet.push_back(new BandMember(BOCCHI)); bandmemberSet.push_back(new BandMember(IJICHI)); bandmemberSet.push_back(new BandMember(YAMADA)); bandmemberSet.push_back(new BandMember(KITA)); for(int i=0;i<ALLEGRO_KEY_MAX;i++)key[i]=false;
}

bool GameWindow::mouse_hover(int sx, int sy, int w, int h) { return (mouse_x>=sx && mouse_x<=sx+w && mouse_y>=sy && mouse_y<=sy+h); }

void GameWindow::game_play() { srand(time(NULL)); game_reset(); game_begin(); int m=-1; while(m!=GAME_EXIT){m=game_run();} show_err_msg(m); }

void GameWindow::show_err_msg(int m) { if(m==GAME_TERMINATE)fprintf(stderr,"Terminated"); else fprintf(stderr,"Err:%d",m); game_destroy(); exit(9); }

GameWindow::GameWindow() {
    if(!al_init())show_err_msg(-1); display=al_create_display(window_width,window_height); event_queue=al_create_event_queue(); timer=al_create_timer(1.0/FPS); if(!timer||!display||!event_queue)show_err_msg(-1); al_init_image_addon(); al_init_font_addon(); al_init_ttf_addon(); al_init_primitives_addon(); al_install_audio(); al_init_acodec_addon(); al_install_keyboard(); al_install_mouse(); font=load_font_safely("src/Caviar_Dreams_Bold.ttf",12,0); Medium_font=load_font_safely("src/Caviar_Dreams_Bold.ttf",24,0); Large_font=load_font_safely("src/Caviar_Dreams_Bold.ttf",36,0); al_register_event_source(event_queue,al_get_display_event_source(display)); al_register_event_source(event_queue,al_get_keyboard_event_source()); al_register_event_source(event_queue,al_get_mouse_event_source()); al_register_event_source(event_queue,al_get_timer_event_source(timer)); game_init();
}

void GameWindow::game_begin() { draw_menu(); al_play_sample_instance(backgroundSound); al_start_timer(timer); }

int GameWindow::game_run() { int e=GAME_CONTINUE; if(!al_is_event_queue_empty(event_queue))e=process_event(); return e; }

int GameWindow::game_update() {
    int lw=level->getWidth(); if(menu->if_gaame_started()){ if(key[ALLEGRO_KEY_W]){bandmemberSet[player1]->changeDir(UP);moving1=true;}else if(key[ALLEGRO_KEY_S]){bandmemberSet[player1]->changeDir(DOWN);moving1=true;}else if(key[ALLEGRO_KEY_A]){bandmemberSet[player1]->changeDir(LEFT);moving1=true;}else if(key[ALLEGRO_KEY_D]){bandmemberSet[player1]->changeDir(RIGHT);moving1=true;}else moving1=false; if(two_player){if(key[ALLEGRO_KEY_UP]){bandmemberSet[player2]->changeDir(UP);moving2=true;}else if(key[ALLEGRO_KEY_DOWN]){bandmemberSet[player2]->changeDir(DOWN);moving2=true;}else if(key[ALLEGRO_KEY_LEFT]){bandmemberSet[player2]->changeDir(LEFT);moving2=true;}else if(key[ALLEGRO_KEY_RIGHT]){bandmemberSet[player2]->changeDir(RIGHT);moving2=true;}else moving2=false;}else update_ai(player2,moving2); update_ai(player3,moving3); update_smart_ai(player4,moving4); menu->Change_Time(); }
    for(int i=0;i<4;i++){if(bandmemberSet[i])bandmemberSet[i]->change_counter(); if(bandmemberSet[i] && bandmemberSet[i]->getSCounter()<=0)bandmemberSet[i]->SpeedChange(1);}
    for(auto b:bombSet)if(b)b->change_counter();
    if(moving1 && can_move(player1,bandmemberSet[player1]->getDir())){ bandmemberSet[player1]->Move(); int idx=(bandmemberSet[player1]->getY()/40*lw)+(bandmemberSet[player1]->getX()/40); if(level->have_speed(idx)){level->speed_disappear(idx);bandmemberSet[player1]->SpeedChange(2);} }
    if(level->is_bombing(bandmemberSet[player1]->getY()/40*lw+bandmemberSet[player1]->getX()/40))level->clear_color(player1);
    if(moving2 && can_move(player2,bandmemberSet[player2]->getDir())){ bandmemberSet[player2]->Move(); int idx=(bandmemberSet[player2]->getY()/40*lw)+(bandmemberSet[player2]->getX()/40); if(level->have_speed(idx)){level->speed_disappear(idx);bandmemberSet[player2]->SpeedChange(2);} }
    if(level->is_bombing(bandmemberSet[player2]->getY()/40*lw+bandmemberSet[player2]->getX()/40) && two_player)level->clear_color(player2);
    if(moving3 && can_move(player3,bandmemberSet[player3]->getDir())){ bandmemberSet[player3]->Move(); int idx=(bandmemberSet[player3]->getY()/40*lw)+(bandmemberSet[player3]->getX()/40); if(level->have_speed(idx)){level->speed_disappear(idx);bandmemberSet[player3]->SpeedChange(2);} }
    if(moving4 && can_move(player4,bandmemberSet[player4]->getDir())){ bandmemberSet[player4]->Move(); int idx=(bandmemberSet[player4]->getY()/40*lw)+(bandmemberSet[player4]->getX()/40); if(level->have_speed(idx)){level->speed_disappear(idx);bandmemberSet[player4]->SpeedChange(2);} }
    return GAME_CONTINUE;
}

void GameWindow::game_reset() { for(auto b:bandmemberSet)if(b)delete b; bandmemberSet.clear(); for(auto b:bombSet)if(b)delete b; bombSet.clear(); moving1=moving2=moving3=moving4=mute=redraw=false; menu->Reset(); for(int i=0;i<4;i++)bandmemberSet.push_back(new BandMember(i)); al_stop_sample_instance(backgroundSound); al_stop_timer(timer); }

void GameWindow::game_destroy() { game_reset(); if(display)al_destroy_display(display); if(event_queue)al_destroy_event_queue(event_queue); if(font)al_destroy_font(font); if(Medium_font)al_destroy_font(Medium_font); if(Large_font)al_destroy_font(Large_font); if(timer)al_destroy_timer(timer); if(icon)al_destroy_bitmap(icon); if(background1)al_destroy_bitmap(background1); if(background2)al_destroy_bitmap(background2); if(background3)al_destroy_bitmap(background3); if(background_menu)al_destroy_bitmap(background_menu); if(background_charchoose)al_destroy_bitmap(background_charchoose); if(background_charstory)al_destroy_bitmap(background_charstory); if(background_introduce)al_destroy_bitmap(background_introduce); if(background_pause)al_destroy_bitmap(background_pause); if(background_win)al_destroy_bitmap(background_win); if(background_lose)al_destroy_bitmap(background_lose); if(background_P1_P2)al_destroy_bitmap(background_P1_P2); if(background_P2_P1)al_destroy_bitmap(background_P2_P1); if(background_P2_P1bocchi)al_destroy_bitmap(background_P2_P1bocchi); if(background_P2_P1ijichi)al_destroy_bitmap(background_P2_P1ijichi); if(background_P2_P1kita)al_destroy_bitmap(background_P2_P1kita); if(background_P2_P1yamada)al_destroy_bitmap(background_P2_P1yamada); if(background_mapchoose)al_destroy_bitmap(background_mapchoose); if(speedtool)al_destroy_bitmap(speedtool); if(wall_school)al_destroy_bitmap(wall_school); if(stone_school)al_destroy_bitmap(stone_school); if(wall_home)al_destroy_bitmap(wall_home); if(stone_home)al_destroy_bitmap(stone_home); if(wall_starry)al_destroy_bitmap(wall_starry); if(stone_starry)al_destroy_bitmap(stone_starry); if(sample)al_destroy_sample(sample); if(backgroundSound)al_destroy_sample_instance(backgroundSound); if(level)delete level; if(menu)delete menu; }

void GameWindow::enter_game(int mid) { game_reset(); level->setLevel(mid); int lw=level->getWidth(); for(auto b:bandmemberSet)if(b)b->Reset(lw); moving1=moving2=moving3=moving4=false; if(mid==1)scene=GAMESCHOOL; else if(mid==2)scene=GAMEHOME; else if(mid==3)scene=GAMESTARRY; menu->set_game_started(true); if(!mute)al_play_sample_instance(backgroundSound); al_start_timer(timer); }

bool GameWindow::can_move(int pi, int d) { if(bandmemberSet.empty()||pi<0||pi>=4||!bandmemberSet[pi])return false; int nx=bandmemberSet[pi]->getX()+3*axis_x[d], ny=bandmemberSet[pi]->getY()+3*axis_y[d], lw=level->getWidth(); int xl=nx-12, xr=nx+11, yt=ny-12, yb=ny+11; int t1=xl/40+(yt/40)*lw, t2=xl/40+(yb/40)*lw, t3=xr/40+(yt/40)*lw, t4=xr/40+(yb/40)*lw; return (level->isRoad(t1)&&level->isRoad(t2)&&level->isRoad(t3)&&level->isRoad(t4)); }

void GameWindow::update_ai(int pi, bool &mf) {
    if(bandmemberSet.empty()||pi<0||pi>=4||!bandmemberSet[pi])return; int cd=bandmemberSet[pi]->getDir(), ax=bandmemberSet[pi]->getX(), ay=bandmemberSet[pi]->getY(), ddir=-1; float md=999999;
    for(auto b:bombSet) if(b&&b->get_counter()>60){ float dx=(float)b->getX()-ax, dy=(float)b->getY()-ay, d2=dx*dx+dy*dy; if(d2<4800){ if(d2<md){md=d2; float adx=(dx>0)?dx:-dx, ady=(dy>0)?dy:-dy; if(adx>ady)ddir=(dx>0)?LEFT:RIGHT; else ddir=(dy>0)?UP:DOWN;} } }
    if(ddir!=-1 && can_move(pi,ddir)){ bandmemberSet[pi]->changeDir(ddir); mf=true; }
    else if(!can_move(pi,cd)||(rand()%20==0)){ std::vector<int> v; for(int d=0;d<4;d++)if(can_move(pi,d))v.push_back(d); if(!v.empty()){bandmemberSet[pi]->changeDir(v[rand()%v.size()]);mf=true;}else mf=false; } else mf=true;
    if(rand()%120==0){ int gx=(ax+20)/40, gy=(ay+20)/40, idx=gy*level->getWidth()+gx; bool st=false; int adj[]={idx-1,idx+1,idx-level->getWidth(),idx+level->getWidth()}; for(int a:adj)if(level->isStone(a)){st=true;break;} if(st){bool bmd=false;for(auto b:bombSet)if(b->getX()==gx*40&&b->getY()==gy*40){bmd=true;break;} if(!bmd)bombSet.push_back(new Bomb(ax,ay,BandMemberClass[pi]));} }
}

int GameWindow::process_event() {
    int inst=GAME_CONTINUE; al_wait_for_event(event_queue,&event); redraw=false;
    if(event.type==ALLEGRO_EVENT_TIMER){if(event.timer.source==timer)redraw=true;}
    else if(event.type==ALLEGRO_EVENT_DISPLAY_CLOSE)return GAME_EXIT;
    else if(event.type==ALLEGRO_EVENT_KEY_DOWN && scene>=GAMESCHOOL && scene<=GAMESTARRY){ key[event.keyboard.keycode]=true;
        switch(event.keyboard.keycode){
            case ALLEGRO_KEY_P: if(al_get_timer_started(timer)){al_stop_timer(timer);cur_scene=scene;scene=GAMEPAUSE;redraw=true;}else{scene=cur_scene;al_start_timer(timer);}break;
            case ALLEGRO_KEY_M: mute=!mute; if(mute)al_stop_sample_instance(backgroundSound); else al_play_sample_instance(backgroundSound); break;
            case ALLEGRO_KEY_SPACE: if(bandmemberSet[player1])bombSet.push_back(new Bomb(bandmemberSet[player1]->getX(),bandmemberSet[player1]->getY(),BandMemberClass[player1])); break;
            case ALLEGRO_KEY_ENTER: if(two_player && bandmemberSet[player2])bombSet.push_back(new Bomb(bandmemberSet[player2]->getX(),bandmemberSet[player2]->getY(),BandMemberClass[player2])); break;
        }
    }else if(event.type==ALLEGRO_EVENT_KEY_UP && scene>=GAMESCHOOL && scene<=GAMESTARRY)key[event.keyboard.keycode]=false;
    else if(event.type==ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){ if(event.mouse.button==1){
        switch(scene){
            case MENU: if(mouse_hover(69,339,90,49))scene=P1_P2; else if(mouse_hover(232,341,122,47))scene=INTRODUCE; else if(mouse_hover(441,346,119,42))scene=CHARSTORY; else if(mouse_hover(659,346,77,42))return GAME_EXIT; break;
            case INTRODUCE: if(mouse_hover(20,400,175,175))scene=MENU; break;
            case CHARCHOOSE: if(mouse_hover(34,39,70,67))scene=MENU; else { if(mouse_hover(69,364,96,36)){player1=BOCCHI;scene=MAPCHOOSE;menu->Load(BOCCHI,IJICHI,YAMADA,KITA);} else if(mouse_hover(232,364,122,36)){player1=YAMADA;scene=MAPCHOOSE;menu->Load(YAMADA,IJICHI,BOCCHI,KITA);} else if(mouse_hover(441,364,119,36)){player1=IJICHI;scene=MAPCHOOSE;menu->Load(IJICHI,BOCCHI,YAMADA,KITA);} else if(mouse_hover(659,364,77,36)){player1=KITA;scene=MAPCHOOSE;menu->Load(KITA,IJICHI,YAMADA,BOCCHI);} } break;
            case CHARSTORY: if(mouse_hover(19,516,75,73))scene=MENU; break;
            case GAMEPAUSE: if(mouse_hover(295,194,499,147)){scene=cur_scene;al_start_timer(timer);} else if(mouse_hover(295,362,499,125)){scene=MENU;al_start_timer(timer);} break;
            case P1_P2: if(mouse_hover(0,0,400,600)){scene=CHARCHOOSE;two_player=false;} else if(mouse_hover(400,0,400,600)){scene=P2_P1;two_player=true;} break;
            case P2_P1: if(mouse_hover(34,39,70,67))scene=MENU; else if(mouse_hover(69,364,96,36)){player1=BOCCHI;scene=P2_P1BOCCHI;} else if(mouse_hover(275,364,85,36)){player1=YAMADA;scene=P2_P1YAMADA;} else if(mouse_hover(466,364,118,36)){player1=IJICHI;scene=P2_P1IJICHI;} else if(mouse_hover(666,364,95,36)){player1=KITA;scene=P2_P1KITA;} break;
            case P2_P1BOCCHI: if(mouse_hover(34,39,70,67))scene=P2_P1; else if(mouse_hover(275,364,85,36)){player2=YAMADA;player3=IJICHI;player4=KITA;menu->Load(BOCCHI,YAMADA,IJICHI,KITA);scene=MAPCHOOSE;} else if(mouse_hover(466,364,118,36)){player2=IJICHI;player3=YAMADA;player4=KITA;menu->Load(BOCCHI,IJICHI,YAMADA,KITA);scene=MAPCHOOSE;} else if(mouse_hover(666,364,95,36)){player2=KITA;player3=IJICHI;player4=YAMADA;menu->Load(BOCCHI,KITA,IJICHI,YAMADA);scene=MAPCHOOSE;} break;
            case P2_P1YAMADA: if(mouse_hover(34,39,70,67))scene=P2_P1; else if(mouse_hover(69,364,96,36)){player2=BOCCHI;player3=IJICHI;player4=KITA;menu->Load(YAMADA,BOCCHI,IJICHI,KITA);scene=MAPCHOOSE;} else if(mouse_hover(466,364,118,36)){player2=IJICHI;player3=BOCCHI;player4=KITA;menu->Load(YAMADA,IJICHI,BOCCHI,KITA);scene=MAPCHOOSE;} else if(mouse_hover(666,364,95,36)){player2=KITA;player3=IJICHI;player4=BOCCHI;menu->Load(YAMADA,KITA,IJICHI,BOCCHI);scene=MAPCHOOSE;} break;
            case P2_P1IJICHI: if(mouse_hover(34,39,70,67))scene=P2_P1; else if(mouse_hover(69,364,96,36)){player2=BOCCHI;player3=YAMADA;player4=KITA;menu->Load(IJICHI,BOCCHI,YAMADA,KITA);scene=MAPCHOOSE;} else if(mouse_hover(275,364,85,36)){player2=YAMADA;player3=BOCCHI;player4=KITA;menu->Load(IJICHI,YAMADA,BOCCHI,KITA);scene=MAPCHOOSE;} else if(mouse_hover(666,364,95,36)){player2=KITA;player3=YAMADA;player4=BOCCHI;menu->Load(IJICHI,KITA,YAMADA,BOCCHI);scene=MAPCHOOSE;} break;
            case P2_P1KITA: if(mouse_hover(34,39,70,67))scene=P2_P1; else if(mouse_hover(69,364,96,36)){player2=BOCCHI;player3=IJICHI;player4=YAMADA;menu->Load(KITA,BOCCHI,IJICHI,YAMADA);scene=MAPCHOOSE;} else if(mouse_hover(275,364,85,36)){player2=YAMADA;player3=IJICHI;player4=BOCCHI;menu->Load(KITA,YAMADA,IJICHI,BOCCHI);scene=MAPCHOOSE;} else if(mouse_hover(466,364,118,36)){player2=IJICHI;player3=BOCCHI;player4=YAMADA;menu->Load(KITA,IJICHI,BOCCHI,YAMADA);scene=MAPCHOOSE;} break;
            case MAPCHOOSE: if(mouse_hover(0,0,800,200))enter_game(1); else if(mouse_hover(0,200,800,200))enter_game(2); else if(mouse_hover(0,400,800,200))enter_game(3); break;
            case GAMEEND: if(mouse_hover(0,540,188,40)){scene=MENU;menu->set_game_started(false);} else if(mouse_hover(694,540,91,40))return GAME_EXIT; break;
        }
    } } else if(event.type==ALLEGRO_EVENT_MOUSE_AXES){mouse_x=event.mouse.x;mouse_y=event.mouse.y;}
    if(redraw){ switch(scene){ case MENU:draw_menu();break; case CHARCHOOSE:draw_charchoose();break; case CHARSTORY:draw_charstory();break; case INTRODUCE:draw_introduce();break; case P1_P2:draw_P1_P2();break; case P2_P1:draw_P2_P1();break; case P2_P1BOCCHI:draw_P2_P1bocchi();break; case P2_P1YAMADA:draw_P2_P1yamada();break; case P2_P1IJICHI:draw_P2_P1ijichi();break; case P2_P1KITA:draw_P2_P1kita();break; case MAPCHOOSE:draw_mapchoose();break; case GAMESCHOOL:if(menu->getTime()<=0)scene=GAMEEND; inst=game_update();draw_running_map(GAMESCHOOL);break; case GAMEHOME:if(menu->getTime()<=0)scene=GAMEEND; inst=game_update();draw_running_map(GAMEHOME);break; case GAMESTARRY:if(menu->getTime()<=0)scene=GAMEEND; inst=game_update();draw_running_map(GAMESTARRY);break; case GAMEPAUSE:draw_pause();break; case GAMEEND:draw_end();break; } redraw=false; }
    if(scene==GAMEPAUSE)draw_pause(); return inst;
}

void GameWindow::draw_running_map(int s) {
    unsigned i,j; ALLEGRO_BITMAP *cw=NULL,*cs=NULL; al_clear_to_color(al_map_rgb(100,100,100)); if(bandmemberSet.empty()||player1<0||player1>=4||!bandmemberSet[player1])return; int px=bandmemberSet[player1]->getX(),py=bandmemberSet[player1]->getY(),lw=level->getWidth(),lh=level->getHeight(),mw=lw*40,mh=lh*40; cam_x=px-300; cam_y=py-300; if(cam_x<0)cam_x=0; if(cam_y<0)cam_y=0; if(cam_x>mw-600)cam_x=mw-600; if(cam_y>mh-600)cam_y=mh-600; if(mw<=600)cam_x=(mw-600)/2; if(mh<=600)cam_y=(mh-600)/2; ALLEGRO_TRANSFORM t; al_identity_transform(&t); al_translate_transform(&t,-cam_x,-cam_y); al_use_transform(&t); switch(s){ case GAMESCHOOL:al_draw_bitmap(background1,0,0,0);cw=wall_school;cs=stone_school;break; case GAMEHOME:al_draw_bitmap(background2,0,0,0);cw=wall_home;cs=stone_home;break; case GAMESTARRY:al_draw_bitmap(background3,0,0,0);cw=wall_starry;cs=stone_starry;break; }
    int sc1=0,sc2=0,sc3=0,sc4=0; for(i=0;i<(unsigned)lh;i++)for(j=0;j<(unsigned)lw;j++){ int idx=i*lw+j; if(level->isStone(idx)){if(cs)al_draw_bitmap(cs,j*40,i*40,0);} else if(level->isRoad(idx)){ if(!level->haveColor(idx))al_draw_filled_rectangle(j*40,i*40,j*40+40,i*40+40,WHITE); else { int c=level->get_character(idx); if(c==BOCCHI){al_draw_filled_rectangle(j*40,i*40,j*40+40,i*40+40,PURPLE);sc1+=50*(player1==BOCCHI);sc2+=50*(player2==BOCCHI);sc3+=50*(player3==BOCCHI);sc4+=50*(player4==BOCCHI);} else if(c==IJICHI){al_draw_filled_rectangle(j*40,i*40,j*40+40,i*40+40,YELLOW);sc1+=50*(player1==IJICHI);sc2+=50*(player2==IJICHI);sc3+=50*(player3==IJICHI);sc4+=50*(player4==IJICHI);} else if(c==YAMADA){al_draw_filled_rectangle(j*40,i*40,j*40+40,i*40+40,BLUE);sc1+=50*(player1==YAMADA);sc2+=50*(player2==YAMADA);sc3+=50*(player3==YAMADA);sc4+=50*(player4==YAMADA);} else if(c==KITA){al_draw_filled_rectangle(j*40,i*40,j*40+40,i*40+40,RED);sc1+=50*(player1==KITA);sc2+=50*(player2==KITA);sc3+=50*(player3==KITA);sc4+=50*(player4==KITA);} } } else { al_draw_filled_rectangle(j*40,i*40,j*40+40,i*40+40,WHITE); if(cw)al_draw_bitmap(cw,j*40,i*40,0); } if(level->have_speed(idx))al_draw_bitmap(speedtool,j*40+5,i*40+5,0); }
    menu->Renew_Score1(sc1); menu->Renew_Score2(sc2); menu->Renew_Score3(sc3); menu->Renew_Score4(sc4); for(auto b:bandmemberSet)if(b)b->Draw(); 
    for(auto b=bombSet.begin();b!=bombSet.end();){ if((*b)->get_counter()<=135){(*b)->Draw();b++;} else if((*b)->get_counter()<=190){ bool dir[4]; int nx=(*b)->getX(),ny=(*b)->getY(); for(int d=0;d<4;d++){ int gi=((ny+axis_y[d]*40)/40*lw)+((nx+axis_x[d]*40)/40); level->bomb_yes(gi); dir[d]=level->isRoad(gi)||level->isStone(gi); } (*b)->DrawFire(dir[0],dir[1],dir[2],dir[3]); b++; } else { int gi=((*b)->getY()/40*lw)+((*b)->getX()/40); int adj[]={gi-1,gi+1,gi-lw,gi+lw}; for(int a:adj){if(level->isStone(a)){level->stone_bomb(a);level->speed_emerge(a);}level->change_character(a,(*b)->get_character()); level->bomb_not(a);} level->change_character(gi,(*b)->get_character()); delete(*b); b=bombSet.erase(b); } }
    al_identity_transform(&t); al_use_transform(&t); al_draw_filled_rectangle(600,0,800,600,al_map_rgb(100,100,100)); menu->Draw(); al_flip_display();
}

void GameWindow::draw_menu() { al_clear_to_color(al_map_rgb(100,100,100)); al_draw_bitmap(background_menu,0,0,0); al_flip_display(); }
void GameWindow::draw_charchoose() { al_clear_to_color(al_map_rgb(100,100,100)); al_draw_bitmap(background_charchoose,0,0,0); al_flip_display(); }
void GameWindow::draw_charstory() { al_clear_to_color(al_map_rgb(100,100,100)); al_draw_bitmap(background_charstory,0,0,0); al_flip_display(); }
void GameWindow::draw_introduce() { al_clear_to_color(al_map_rgb(100,100,100)); al_draw_bitmap(background_introduce,0,0,0); al_flip_display(); }
void GameWindow::draw_P1_P2() { al_clear_to_color(al_map_rgb(100,100,100)); al_draw_bitmap(background_P1_P2,0,0,0); al_flip_display(); }
void GameWindow::draw_P2_P1() { al_clear_to_color(al_map_rgb(100,100,100)); al_draw_bitmap(background_P2_P1,0,0,0); al_flip_display(); }
void GameWindow::draw_P2_P1bocchi() { al_clear_to_color(al_map_rgb(100,100,100)); al_draw_bitmap(background_P2_P1bocchi,0,0,0); al_flip_display(); }
void GameWindow::draw_P2_P1yamada() { al_clear_to_color(al_map_rgb(100,100,100)); al_draw_bitmap(background_P2_P1yamada,0,0,0); al_flip_display(); }
void GameWindow::draw_P2_P1ijichi() { al_clear_to_color(al_map_rgb(100,100,100)); al_draw_bitmap(background_P2_P1ijichi,0,0,0); al_flip_display(); }
void GameWindow::draw_P2_P1kita() { al_clear_to_color(al_map_rgb(100,100,100)); al_draw_bitmap(background_P2_P1kita,0,0,0); al_flip_display(); }
void GameWindow::draw_mapchoose() { al_clear_to_color(al_map_rgb(100,100,100)); al_draw_bitmap(background_mapchoose,0,0,0); al_flip_display(); }
void GameWindow::draw_pause() { al_clear_to_color(al_map_rgb(100,100,100)); al_draw_bitmap(background_pause,0,0,0); al_flip_display(); }
void GameWindow::draw_end() { if(menu->win()){al_clear_to_color(al_map_rgb(100,100,100));al_draw_bitmap(background_win,0,0,0);} else {al_clear_to_color(al_map_rgb(100,100,100));al_draw_bitmap(background_lose,0,0,0);} al_flip_display(); }

void GameWindow::update_smart_ai(int pi, bool &mf) {
    if(bandmemberSet.empty()||pi<0||pi>=(int)bandmemberSet.size()||!bandmemberSet[pi])return;
    int ax=bandmemberSet[pi]->getX(),ay=bandmemberSet[pi]->getY(),gx=(ax+20)/40,gy=(ay+20)/40,lw=level->getWidth(),ai=gy*lw+gx,ddir=-1; float md=999999;
    for(auto b:bombSet)if(b&&b->get_counter()>40){ float dx=(float)b->getX()-ax,dy=(float)b->getY()-ay,d2=dx*dx+dy*dy; if(d2<4000){if(d2<md){md=d2;float adx=(dx>0)?dx:-dx,ady=(dy>0)?dy:-dy; if(adx>ady)ddir=(dx>0)?LEFT:RIGHT; else ddir=(dy>0)?UP:DOWN;}} }
    if(ddir!=-1 && can_move(pi,ddir)){ bandmemberSet[pi]->changeDir(ddir); mf=true; return; }
    if(!bandmemberSet[player1])return;
    int ti=(bandmemberSet[player1]->getY()+20)/40*lw+(bandmemberSet[player1]->getX()+20)/40;
    if(ai!=ti){ std::queue<int> q; std::vector<int> par(NumOfGrid,-1); std::vector<bool> vis(NumOfGrid,false); q.push(ai); vis[ai]=true; bool found=false;
        while(!q.empty()){ int c=q.front(); q.pop(); if(c==ti){found=true;break;} 
            for(int d=0;d<4;d++){ int nx=(c%lw)+axis_x[d],ny=(c/lw)+axis_y[d],ni=ny*lw+nx; if(nx>=0&&nx<lw&&ny>=0&&ny<level->getHeight()&&!vis[ni]&&level->isRoad(ni)){vis[ni]=true;par[ni]=c;q.push(ni);} } }
        if(found){ int s=ti; while(par[s]!=-1&&par[s]!=ai)s=par[s]; int sx=s%lw,sy=s/lw,dir=-1; if(sx<gx)dir=LEFT; else if(sx>gx)dir=RIGHT; else if(sy<gy)dir=UP; else if(sy>gy)dir=DOWN; if(dir!=-1&&can_move(pi,dir)){bandmemberSet[pi]->changeDir(dir);mf=true;} } else update_ai(pi,mf);
    }
    bool t=false; for(int d=0;d<4;d++){ int nx=gx+axis_x[d],ny=gy+axis_y[d],ni=ny*lw+nx; if(nx>=0&&nx<lw&&ny>=0&&ny<level->getHeight()&&(level->isStone(ni)||(level->haveColor(ni)&&level->get_character(ni)!=pi))){t=true;break;} }
    if(t&&(rand()%60==0)&&(level->get_character(ai)!=pi||!level->haveColor(ai))){ bool bmd=false; for(auto b:bombSet){if(b && b->getX()==gx*40&&b->getY()==gy*40){bmd=true;break;}} if(!bmd)bombSet.push_back(new Bomb(ax,ay,BandMemberClass[pi])); }
}
