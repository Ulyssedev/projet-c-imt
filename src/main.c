#include "game.h"
#include <SDL2/SDL.h>

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window *win = SDL_CreateWindow("Zelda IMT", SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH,
                                     WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
  SDL_Renderer *ren = SDL_CreateRenderer(
      win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  GameState game;
  game_init(&game, ren);

  SDL_Event e;
  while (game.running) {
    while (SDL_PollEvent(&e)) {
      game_handle_input(&game, &e);
    }

    game_update(&game);

    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderClear(ren);

    game_render(&game, ren);

    SDL_RenderPresent(ren);
  }

  game_cleanup(&game);
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();

  return 0;
}
