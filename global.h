#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

#include <allegro5/allegro_font.h>

#define font_size 12
#define grid_width 40
#define grid_height 40
#define member_width 36

#define window_width 800
#define window_height 600
#define field_width 600
#define field_height 600

#define Num_TowerType 0
#define Num_MonsterType 0
#define NumOfGrid (field_width / grid_width) * (field_height / grid_height)
#define Num_BandMemberType 4

extern int TowerRadius[];
extern char TowerClass[][20];
extern int TowerWidth[];
extern int TowerHeight[];

extern int BandMemberRadius[];//
extern char BandMemberClass[][20];
extern int BandMemberWidth[];
extern int BandMemberHeight[];//

enum {ARCANE = 0, ARCHER, CANON, POISON, STORM};
enum {WOLF = 0, WOLFKNIGHT, DEMONNIJIA, CAVEMAN};
enum {BOCCHI = 0, IJICHI, YAMADA, KITA};

const int axis_x[] = {-1, 1, 0, 0};
const int axis_y[] = {0, 0, -1, 1};
                  //left right up down
enum {LEFT=0, RIGHT, UP, DOWN};

#endif // GLOBAL_H_INCLUDED
