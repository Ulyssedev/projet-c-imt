#ifndef GAME_H_
#define GAME_H_

#include "ressources.h"
#include "enemy.h"
#include "character.h"
#include <SDL2/SDL.h>

typedef struct {
    int world_map[88][256];
    char blocking_map[88][256];
    SDL_Texture **tile_textures;
    SDL_Texture **link_textures;
    SDL_Texture **link_sword_textures;
    SDL_Texture **enemy_textures;
    SDL_Texture **imt_textures;
    
    Player player;
    int current_room_x;
    int current_room_y;
    
    Enemy enemies[MAX_ENEMIES];
    int enemy_count;
    
    int running;
} GameState;

void game_init(GameState *game, SDL_Renderer *ren);
void game_handle_input(GameState *game, SDL_Event *e);
void game_update(GameState *game);
void game_render(GameState *game, SDL_Renderer *ren);
void game_cleanup(GameState *game);

#endif
