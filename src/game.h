#ifndef GAME_H_
#define GAME_H_

#include "character.h"
#include "enemy.h"
#include "ressources.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

typedef struct {
  int world_map[88][256];
  char blocking_map[88][256];
  SDL_Texture **tile_textures;
  SDL_Texture **link_textures;
  SDL_Texture **link_sword_textures;
  SDL_Texture **enemy_textures;
  SDL_Texture **imt_textures;
  SDL_Texture **heart_textures;

  Mix_Music *aie_sound;
  Mix_Music *aie_e_sound;

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
