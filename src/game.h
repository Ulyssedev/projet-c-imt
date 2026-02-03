#ifndef GAME_H_
#define GAME_H_

#include "ressources.h"
#include <SDL2/SDL.h>

typedef struct {
    int world_map[88][256];
    char blocking_map[88][256];
    SDL_Texture **tile_textures;
    SDL_Texture **link_textures;
    int current_room_x;
    int current_room_y;
    float player_x;
    float player_y;
    int player_direction;
    int running;
} GameState;

void game_init(GameState *game, SDL_Renderer *ren);
void game_handle_input(GameState *game, SDL_Event *e);
void game_update(GameState *game);
void game_render(GameState *game, SDL_Renderer *ren);
void game_cleanup(GameState *game);

#endif
