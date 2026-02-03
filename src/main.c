#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include "character.h"
#include "link.h"
#include "enemy.h"
#include "map.h"
#include "game.h"
int main(int argc, char *argv[]) {
  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init(); // Initialize SDL_ttf

  SDL_Window *win = SDL_CreateWindow("SDL2_ttf Example", 100, 100, 640, 480, 0);
  SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

  // 1. Load a font (Provide a valid path to a .ttf file)
  TTF_Font *font = TTF_OpenFont("src/res/DejaVuSans-Bold.ttf", 28);

  if (!font) {
    // This will tell you if the file path is incorrect
    printf("Failed to load font: %s\n", TTF_GetError());
    // Handle error...
  }
  // 2. Create text surface
  SDL_Color textColor = {255, 255, 255, 255}; // White
  SDL_Surface *textSurface =
      TTF_RenderText_Solid(font, "Hello SDL_ttf!", textColor);

  // 3. Convert surface to texture
  SDL_Texture *textTexture = SDL_CreateTextureFromSurface(ren, textSurface);

  SDL_Rect textRect = {100, 100, textSurface->w, textSurface->h};
  SDL_FreeSurface(textSurface);

  // Main Loop (Simple)
  int running = 1;
  SDL_Event e;
  while (running) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT)
        running = 0;
    }

    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderClear(ren);

    // 4. Draw the text
    SDL_RenderCopy(ren, textTexture, NULL, &textRect);

    SDL_RenderPresent(ren);
  }

  // Cleanup
  SDL_DestroyTexture(textTexture);
  TTF_CloseFont(font);
  TTF_Quit();
  SDL_Quit();
  return 0;
}