#include "game.h"
#include "IOManager.h"
#include <stdlib.h>
#include <time.h>

#define PLAYER_SPEED 3.0f
#define ENEMY_SPEED 1.5f
#define ATTACK_DURATION 300
#define ENEMY_MOVE_INTERVAL 500
#define ENEMY_ANIM_INTERVAL 200

static SDL_Texture **load_link_textures(SDL_Renderer *ren) {
    SDL_Texture **textures = malloc(8 * sizeof(SDL_Texture *));
    char path[64];
    for (int i = 0; i < 8; i++) {
        snprintf(path, sizeof(path), "src/res/link%d.bmp", i);
        textures[i] = loadTexture(path, ren);
    }
    return textures;
}

static SDL_Texture **load_sword_textures(SDL_Renderer *ren) {
    SDL_Texture **textures = malloc(4 * sizeof(SDL_Texture *));
    textures[0] = loadTexture("src/res/linkSwordRight.bmp", ren);
    textures[1] = loadTexture("src/res/linkSwordUp.bmp", ren);
    textures[2] = loadTexture("src/res/linkSwordLeft.bmp", ren);
    textures[3] = loadTexture("src/res/linkSwordDown.bmp", ren);
    return textures;
}

static SDL_Texture **load_enemy_textures(SDL_Renderer *ren) {
    SDL_Texture **textures = malloc(8 * sizeof(SDL_Texture *));
    char path[64];
    for (int i = 0; i < 8; i++) {
        snprintf(path, sizeof(path), "src/res/enemy%d.bmp", i);
        textures[i] = loadTexture(path, ren);
    }
    return textures;
}

static SDL_Texture **load_imt_textures(SDL_Renderer *ren) {
    SDL_Texture **textures = malloc(10 * sizeof(SDL_Texture *));
    char path[64];
    for (int i = 0; i < 10; i++) {
        snprintf(path, sizeof(path), "src/res/imt_%02d.bmp", i);
        textures[i] = loadTexture(path, ren);
    }
    return textures;
}

static void spawn_enemies_for_room(GameState *game) {
    game->enemy_count = 0;
    srand(game->current_room_x * 100 + game->current_room_y);
    
    int num_enemies = rand() % 4 + 1;
    
    for (int i = 0; i < num_enemies && game->enemy_count < MAX_ENEMIES; i++) {
        Enemy *e = &game->enemies[game->enemy_count];
        e->x = 32 + rand() % (ROOM_WIDTH_TILES * 16 - 64);
        e->y = 32 + rand() % (ROOM_HEIGHT_TILES * 16 - 64);
        e->dx = (rand() % 2 == 0) ? ENEMY_SPEED : -ENEMY_SPEED;
        e->dy = (rand() % 2 == 0) ? ENEMY_SPEED : -ENEMY_SPEED;
        e->health = 2;
        e->type = (rand() % 2 == 0) ? ENEMY_TYPE_OCTOROK : ENEMY_TYPE_IMT;
        e->direction = rand() % 4;
        e->active = 1;
        e->anim_frame = 0;
        e->last_anim_time = SDL_GetTicks();
        e->last_move_time = SDL_GetTicks();
        game->enemy_count++;
    }
}

void game_init(GameState *game, SDL_Renderer *ren) {
    srand(time(NULL));
    
    loadMap("src/res/nes_zelda_overworld_tile_map.txt", game->world_map);
    loadBlockingMap("src/res/nes_zelda_overworld_blocking_map.txt", game->blocking_map);
    game->tile_textures = load_MAP_Textures("src/res/overworldtiles.bmp", ren);
    game->link_textures = load_link_textures(ren);
    game->link_sword_textures = load_sword_textures(ren);
    game->enemy_textures = load_enemy_textures(ren);
    game->imt_textures = load_imt_textures(ren);
    
    game->current_room_x = 7;
    game->current_room_y = 7;
    
    game->player.x = ROOM_WIDTH_TILES * 16 / 2.0f;
    game->player.y = ROOM_HEIGHT_TILES * 16 / 2.0f;
    game->player.direction = 0;
    game->player.health = PLAYER_MAX_HEALTH;
    game->player.is_attacking = 0;
    game->player.attack_start_time = 0;
    game->player.last_hit_time = 0;
    
    game->running = 1;
    
    spawn_enemies_for_room(game);
}

void game_handle_input(GameState *game, SDL_Event *e) {
    if (e->type == SDL_QUIT) {
        game->running = 0;
    }
    if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_SPACE) {
        if (!game->player.is_attacking) {
            game->player.is_attacking = 1;
            game->player.attack_start_time = SDL_GetTicks();
        }
    }
}

static int check_collision(float x1, float y1, int w1, int h1, float x2, float y2, int w2, int h2) {
    return x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2;
}

static void get_sword_hitbox(Player *p, int *sx, int *sy, int *sw, int *sh) {
    *sw = 20;
    *sh = 20;
    switch (p->direction) {
        case 0: *sx = p->x + 16; *sy = p->y - 2; *sw = 12; *sh = 20; break;
        case 2: *sx = p->x - 2; *sy = p->y - 16; *sw = 20; *sh = 12; break;
        case 4: *sx = p->x - 12; *sy = p->y - 2; *sw = 12; *sh = 20; break;
        case 6: *sx = p->x - 2; *sy = p->y + 16; *sw = 20; *sh = 12; break;
    }
}

void game_update(GameState *game) {
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    Uint32 now = SDL_GetTicks();
    
    if (game->player.is_attacking && now - game->player.attack_start_time > ATTACK_DURATION) {
        game->player.is_attacking = 0;
    }
    
    float dx = 0, dy = 0;
    if (!game->player.is_attacking) {
        if (keys[SDL_SCANCODE_UP]) { dy = -PLAYER_SPEED; game->player.direction = 2; }
        if (keys[SDL_SCANCODE_DOWN]) { dy = PLAYER_SPEED; game->player.direction = 6; }
        if (keys[SDL_SCANCODE_LEFT]) { dx = -PLAYER_SPEED; game->player.direction = 4; }
        if (keys[SDL_SCANCODE_RIGHT]) { dx = PLAYER_SPEED; game->player.direction = 0; }
    }
    
    float new_x = game->player.x + dx;
    float new_y = game->player.y + dy;
    
    int room_pixel_w = ROOM_WIDTH_TILES * 16;
    int room_pixel_h = ROOM_HEIGHT_TILES * 16;
    
    int start_tile_x = game->current_room_x * ROOM_WIDTH_TILES;
    int start_tile_y = game->current_room_y * ROOM_HEIGHT_TILES;
    
    int can_move_x = 1, can_move_y = 1;
    int hitbox_margin = 4;
    
    if (dx != 0) {
        int check_tile_x = start_tile_x + (int)((new_x + (dx > 0 ? 16 - hitbox_margin : hitbox_margin - 1)) / 16);
        int check_tile_y = start_tile_y + (int)((game->player.y + 8) / 16);
        if (game->blocking_map[check_tile_y][check_tile_x] == 'X') {
            can_move_x = 0;
        }
    }
    
    if (dy != 0) {
        int check_tile_y = start_tile_y + (int)((new_y + (dy > 0 ? 16 - hitbox_margin : hitbox_margin - 1)) / 16);
        int check_tile_x = start_tile_x + (int)((game->player.x + 8) / 16);
        if (game->blocking_map[check_tile_y][check_tile_x] == 'X') {
            can_move_y = 0;
        }
    }
    
    int room_changed = 0;
    if (can_move_x) {
        if (new_x < 0 && game->current_room_x > 0) {
            game->current_room_x--;
            new_x = room_pixel_w - 16;
            room_changed = 1;
        } else if (new_x > room_pixel_w - 16 && game->current_room_x < WORLD_ROOMS_X - 1) {
            game->current_room_x++;
            new_x = 0;
            room_changed = 1;
        }
        if (new_x >= 0 && new_x <= room_pixel_w - 16) game->player.x = new_x;
    }
    
    if (can_move_y) {
        if (new_y < 0 && game->current_room_y > 0) {
            game->current_room_y--;
            new_y = room_pixel_h - 16;
            room_changed = 1;
        } else if (new_y > room_pixel_h - 16 && game->current_room_y < WORLD_ROOMS_Y - 1) {
            game->current_room_y++;
            new_y = 0;
            room_changed = 1;
        }
        if (new_y >= 0 && new_y <= room_pixel_h - 16) game->player.y = new_y;
    }
    
    if (room_changed) {
        spawn_enemies_for_room(game);
    }
    
    for (int i = 0; i < game->enemy_count; i++) {
        Enemy *e = &game->enemies[i];
        if (!e->active) continue;
        
        if (now - e->last_anim_time > ENEMY_ANIM_INTERVAL) {
            e->anim_frame = (e->anim_frame + 1) % 2;
            e->last_anim_time = now;
        }
        
        if (now - e->last_move_time > ENEMY_MOVE_INTERVAL) {
            if (rand() % 3 == 0) {
                e->dx = -e->dx;
            }
            if (rand() % 3 == 0) {
                e->dy = -e->dy;
            }
            e->last_move_time = now;
        }
        
        float next_x = e->x + e->dx;
        float next_y = e->y + e->dy;
        
        if (next_x < 16 || next_x > room_pixel_w - 32) e->dx = -e->dx;
        if (next_y < 16 || next_y > room_pixel_h - 32) e->dy = -e->dy;
        
        e->x += e->dx * 0.5f;
        e->y += e->dy * 0.5f;
        
        if (e->dx > 0) e->direction = 0;
        else if (e->dx < 0) e->direction = 2;
        if (e->dy < 0) e->direction = 1;
        else if (e->dy > 0) e->direction = 3;
        
        if (game->player.is_attacking) {
            int sx, sy, sw, sh;
            get_sword_hitbox(&game->player, &sx, &sy, &sw, &sh);
            if (check_collision(sx, sy, sw, sh, e->x, e->y, 16, 16)) {
                e->health--;
                if (e->health <= 0) {
                    e->active = 0;
                }
            }
        }
        
        if (now - game->player.last_hit_time > INVINCIBILITY_TIME) {
            if (check_collision(game->player.x + 2, game->player.y + 2, 12, 12, e->x, e->y, 16, 16)) {
                game->player.health--;
                game->player.last_hit_time = now;
                if (game->player.health <= 0) {
                    game->player.health = PLAYER_MAX_HEALTH;
                    game->current_room_x = 7;
                    game->current_room_y = 7;
                    game->player.x = ROOM_WIDTH_TILES * 16 / 2.0f;
                    game->player.y = ROOM_HEIGHT_TILES * 16 / 2.0f;
                    spawn_enemies_for_room(game);
                }
            }
        }
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
    
    for (int i = 0; i < game->enemy_count; i++) {
        Enemy *e = &game->enemies[i];
        if (!e->active) continue;
        
        SDL_Texture *tex;
        if (e->type == ENEMY_TYPE_OCTOROK) {
            int tex_idx = e->direction * 2 + e->anim_frame;
            tex = game->enemy_textures[tex_idx % 8];
        } else {
            int tex_idx = e->anim_frame;
            tex = game->imt_textures[tex_idx % 10];
        }
        
        SDL_Rect dst = {
            (int)(e->x * SCALE_FACTOR),
            (int)(e->y * SCALE_FACTOR),
            16 * SCALE_FACTOR,
            16 * SCALE_FACTOR
        };
        SDL_RenderCopy(ren, tex, NULL, &dst);
    }
    
    Uint32 now = SDL_GetTicks();
    int is_flashing = (now - game->player.last_hit_time < INVINCIBILITY_TIME) && ((now / 100) % 2);
    
    if (!is_flashing) {
        if (game->player.is_attacking) {
            int sword_idx = game->player.direction / 2;
            SDL_Texture *sword_tex = game->link_sword_textures[sword_idx];
            
            int w, h;
            SDL_QueryTexture(sword_tex, NULL, NULL, &w, &h);
            
            int px = (int)(game->player.x * SCALE_FACTOR);
            int py = (int)(game->player.y * SCALE_FACTOR);
            
            SDL_Rect dst;
            switch (game->player.direction) {
                case 0: dst = (SDL_Rect){px, py, w * SCALE_FACTOR, h * SCALE_FACTOR}; break;
                case 2: dst = (SDL_Rect){px, py - (h - 16) * SCALE_FACTOR, w * SCALE_FACTOR, h * SCALE_FACTOR}; break;
                case 4: dst = (SDL_Rect){px - (w - 16) * SCALE_FACTOR, py, w * SCALE_FACTOR, h * SCALE_FACTOR}; break;
                case 6: dst = (SDL_Rect){px, py, w * SCALE_FACTOR, h * SCALE_FACTOR}; break;
            }
            SDL_RenderCopy(ren, sword_tex, NULL, &dst);
        } else {
            int frame = (SDL_GetTicks() / 150) % 2;
            int tex_idx = game->player.direction + frame;
            SDL_Rect player_dst = {
                (int)(game->player.x * SCALE_FACTOR),
                (int)(game->player.y * SCALE_FACTOR),
                16 * SCALE_FACTOR,
                16 * SCALE_FACTOR
            };
            SDL_RenderCopy(ren, game->link_textures[tex_idx], NULL, &player_dst);
        }
    }
    
    for (int i = 0; i < game->player.health; i++) {
        SDL_Rect heart = {10 + i * 20, 10, 16, 16};
        SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
        SDL_RenderFillRect(ren, &heart);
    }
}

void game_cleanup(GameState *game) {
    for (int i = 0; i < 160; i++) {
        if (game->tile_textures[i]) SDL_DestroyTexture(game->tile_textures[i]);
    }
    free(game->tile_textures);
    
    for (int i = 0; i < 8; i++) {
        if (game->link_textures[i]) SDL_DestroyTexture(game->link_textures[i]);
    }
    free(game->link_textures);
    
    for (int i = 0; i < 4; i++) {
        if (game->link_sword_textures[i]) SDL_DestroyTexture(game->link_sword_textures[i]);
    }
    free(game->link_sword_textures);
    
    for (int i = 0; i < 8; i++) {
        if (game->enemy_textures[i]) SDL_DestroyTexture(game->enemy_textures[i]);
    }
    free(game->enemy_textures);
    
    for (int i = 0; i < 10; i++) {
        if (game->imt_textures[i]) SDL_DestroyTexture(game->imt_textures[i]);
    }
    free(game->imt_textures);
}
