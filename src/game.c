#include "game.h"
#include "IOManager.h"
#include <stdlib.h>

#define PLAYER_SPEED 3.0f

static SDL_Texture **load_link_textures(SDL_Renderer *ren) {
    SDL_Texture **textures = malloc(8 * sizeof(SDL_Texture *));
    char path[64];
    for (int i = 0; i < 8; i++) {
        snprintf(path, sizeof(path), "src/res/link%d.bmp", i);
        textures[i] = loadTexture(path, ren);
    }
    return textures;
}

void game_init(GameState *game, SDL_Renderer *ren) {
    loadMap("src/res/nes_zelda_overworld_tile_map.txt", game->world_map);
    loadBlockingMap("src/res/nes_zelda_overworld_blocking_map.txt", game->blocking_map);
    game->tile_textures = load_MAP_Textures("src/res/overworldtiles.bmp", ren);
    game->link_textures = load_link_textures(ren);
    
    game->current_room_x = 7;
    game->current_room_y = 7;
    game->player_x = ROOM_WIDTH_TILES * 16 / 2.0f;
    game->player_y = ROOM_HEIGHT_TILES * 16 / 2.0f;
    game->player_direction = 0;
    game->running = 1;
}

void game_handle_input(GameState *game, SDL_Event *e) {
    if (e->type == SDL_QUIT) {
        game->running = 0;
    }
}

void game_update(GameState *game) {
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    float dx = 0, dy = 0;
    if (keys[SDL_SCANCODE_UP]) { dy = -PLAYER_SPEED; game->player_direction = 2; }
    if (keys[SDL_SCANCODE_DOWN]) { dy = PLAYER_SPEED; game->player_direction = 6; }
    if (keys[SDL_SCANCODE_LEFT]) { dx = -PLAYER_SPEED; game->player_direction = 4; }
    if (keys[SDL_SCANCODE_RIGHT]) { dx = PLAYER_SPEED; game->player_direction = 0; }
    
    float new_x = game->player_x + dx;
    float new_y = game->player_y + dy;
    
    int room_pixel_w = ROOM_WIDTH_TILES * 16;
    int room_pixel_h = ROOM_HEIGHT_TILES * 16;
    
    int start_tile_x = game->current_room_x * ROOM_WIDTH_TILES;
    int start_tile_y = game->current_room_y * ROOM_HEIGHT_TILES;
    
    int can_move_x = 1, can_move_y = 1;
    int hitbox_margin = 4;
    
    if (dx != 0) {
        int check_tile_x = start_tile_x + (int)((new_x + (dx > 0 ? 16 - hitbox_margin : hitbox_margin - 1)) / 16);
        int check_tile_y = start_tile_y + (int)((game->player_y + 8) / 16);
        if (game->blocking_map[check_tile_y][check_tile_x] == 'X') {
            can_move_x = 0;
        }
    }
    
    if (dy != 0) {
        int check_tile_y = start_tile_y + (int)((new_y + (dy > 0 ? 16 - hitbox_margin : hitbox_margin - 1)) / 16);
        int check_tile_x = start_tile_x + (int)((game->player_x + 8) / 16);
        if (game->blocking_map[check_tile_y][check_tile_x] == 'X') {
            can_move_y = 0;
        }
    }
    
    if (can_move_x) {
        if (new_x < 0 && game->current_room_x > 0) {
            game->current_room_x--;
            new_x = room_pixel_w - 16;
        } else if (new_x > room_pixel_w - 16 && game->current_room_x < WORLD_ROOMS_X - 1) {
            game->current_room_x++;
            new_x = 0;
        }
        if (new_x >= 0 && new_x <= room_pixel_w - 16) game->player_x = new_x;
    }
    
    if (can_move_y) {
        if (new_y < 0 && game->current_room_y > 0) {
            game->current_room_y--;
            new_y = room_pixel_h - 16;
        } else if (new_y > room_pixel_h - 16 && game->current_room_y < WORLD_ROOMS_Y - 1) {
            game->current_room_y++;
            new_y = 0;
        }
        if (new_y >= 0 && new_y <= room_pixel_h - 16) game->player_y = new_y;
    }
}

void game_render(GameState *game, SDL_Renderer *ren) {
    int start_tile_x = game->current_room_x * ROOM_WIDTH_TILES;
    int start_tile_y = game->current_room_y * ROOM_HEIGHT_TILES;
    
    for (int ty = 0; ty < ROOM_HEIGHT_TILES; ty++) {
        for (int tx = 0; tx < ROOM_WIDTH_TILES; tx++) {
            int map_x = start_tile_x + tx;
            int map_y = start_tile_y + ty;
            int tile_id = game->world_map[map_y][map_x];
            
            SDL_Rect dst = {
                tx * 16 * SCALE_FACTOR,
                ty * 16 * SCALE_FACTOR,
                16 * SCALE_FACTOR,
                16 * SCALE_FACTOR
            };
            SDL_RenderCopy(ren, game->tile_textures[tile_id], NULL, &dst);
        }
    }
    
    int frame = (SDL_GetTicks() / 150) % 2;
    int base_sprite = game->player_direction;
    int tex_idx = base_sprite + frame;
    SDL_Rect player_dst = {
        (int)(game->player_x * SCALE_FACTOR),
        (int)(game->player_y * SCALE_FACTOR),
        16 * SCALE_FACTOR,
        16 * SCALE_FACTOR
    };
    SDL_RenderCopy(ren, game->link_textures[tex_idx], NULL, &player_dst);
}

void game_cleanup(GameState *game) {
    for (int i = 0; i < 256; i++) {
        if (game->tile_textures[i]) SDL_DestroyTexture(game->tile_textures[i]);
    }
    free(game->tile_textures);
    
    for (int i = 0; i < 8; i++) {
        if (game->link_textures[i]) SDL_DestroyTexture(game->link_textures[i]);
    }
    free(game->link_textures);
}
