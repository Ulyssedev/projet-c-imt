#ifndef ENEMY_H_
#define ENEMY_H_

#include <SDL2/SDL.h>

#define MAX_ENEMIES 10
#define ENEMY_TYPE_OCTOROK 0
#define ENEMY_TYPE_IMT 1

typedef struct {
    float x, y;
    float dx, dy;
    int health;
    int type;
    int direction;
    int active;
    int anim_frame;
    Uint32 last_anim_time;
    Uint32 last_move_time;
} Enemy;

#endif
