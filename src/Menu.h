#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <vector>
#include "Object.h"
#include "Circle.h"
#include "global.h"

const int initial_time = 60 * 100;


class Menu : public Object
{
public:
    Menu(); 
    virtual ~Menu();

    // void change_player(int, int, int, int);

    void Reset();
    void Draw(int, int, int, int);
    void Draw();

    void Load(int, int, int, int);

    // Detect if cursor hovers over any of tower on menu
    // If so, return its type
    // Otherwise, return -1
    // int MouseIn(int, int);

    // static function that detect if one point is on a line
    // This function is just used to simplify "MouseIn"
    static bool isInRange(int, int, int);

    // Check if current coin is not less than needed coin
    // bool Enough_Coin(int);
    // void Change_Coin(int change) { Coin += change; }
    void Change_Time() { Time -= 1; }
    bool Subtract_HP(int escapeNum = 1);
    void Gain_Score(int);

    // int getTowerCoin(int type) { return need_coin[type]; }
    int getScore() { return Score; }
    int getCoin() { return Coin; }
    int getKilled() { return killedMonster; }
    int getTime() { return Time; }

    void Renew_Score1(int x) { Score1 = x; }
    void Renew_Score2(int x) { Score2 = x; }
    void Renew_Score3(int x) { Score3 = x; }
    void Renew_Score4(int x) { Score4 = x; }

    int getScore1() { return Score1; }
    int getScore2() { return Score2; }
    int getScore3() { return Score3; }
    int getScore4() { return Score4; }

    bool if_gaame_started() { return game_started; }
    void set_game_started(bool status) { game_started = status; }
    
    bool win();

private:
    // std::vector<ALLEGRO_BITMAP*> menu_tower;
    // ALLEGRO_BITMAP *love = NULL;
    ALLEGRO_BITMAP *player1_img = NULL;
    ALLEGRO_BITMAP *player2_img = NULL;
    ALLEGRO_BITMAP *player3_img = NULL;
    ALLEGRO_BITMAP *player4_img = NULL;
    ALLEGRO_BITMAP *bocchi_img = NULL;
    ALLEGRO_BITMAP *ijichi_img = NULL;
    ALLEGRO_BITMAP *yamada_img = NULL;
    ALLEGRO_BITMAP *kita_img = NULL;
    ALLEGRO_FONT *menuFont;
    int HealthPoint = 10;
    int killedMonster = 0;
    int Score = 0;
    int Score1 = 0;
    int Score2 = 0;
    int Score3 = 0;
    int Score4 = 0;
    int player1 = BOCCHI;
    int player2 = IJICHI;
    int player3 = YAMADA;
    int player4 = KITA;
    int Coin = 0;
    int selectedTower = -1;
    int Time = initial_time;

    bool game_started = false;
    // int need_coin[Num_TowerType] = {-5, -10, -20, -40, -70};
};

#endif // MENU_H_INCLUDED
