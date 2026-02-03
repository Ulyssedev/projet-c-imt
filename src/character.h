#ifndef CHARACTER_H_
#define CHARACTER_H_

#include <SDL2/SDL.h>

#define PLAYER_MAX_HEALTH 6
#define INVINCIBILITY_TIME 1500

typedef struct {
    float x, y;
    int direction;
    int health;
    int is_attacking;
    Uint32 attack_start_time;
    Uint32 last_hit_time;
} Player;

#endif
