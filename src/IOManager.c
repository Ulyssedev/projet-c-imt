#include "IOManager.h"

extern TTF_Font *Sans;
extern SDL_Color Yellow;

// ############################################################## FONCTIONS
// GENERALES
// #####################################################################

void initSDL() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) // initialisation de la SDL
  {
    printf("Erreur d'initialisation de la SDL : %s", SDL_GetError());
    SDL_Quit();
  }
}

SDL_Window *createWindow(char winName[], int winWidth, int winHeigth) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("SDL_Init Error\n");
  }
  SDL_Window *win =
      SDL_CreateWindow(winName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       winWidth, winHeigth, SDL_WINDOW_SHOWN);
  if (win == NULL) {
    printf("SDL_CreateWindow Error\n");
    SDL_Quit();
  }
  return win;
}

SDL_Renderer *createRenderer(SDL_Window *win) {
  SDL_Renderer *ren = SDL_CreateRenderer(
      win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (ren == NULL) {
    SDL_DestroyWindow(win);
    printf("SDL_CreateRenderer Error");
    SDL_Quit();
  }
  return ren;
}

void updateDisplay(SDL_Renderer *ren) { SDL_RenderPresent(ren); }

void clearRenderer(SDL_Renderer *ren) { SDL_RenderClear(ren); }

void QuitSDL(SDL_Window *win, SDL_Renderer *ren) {
  SDL_DestroyWindow(win);
  SDL_DestroyRenderer(ren);
  SDL_Quit();
}

SDL_Texture *loadTexture(const char *file, SDL_Renderer *ren) {
  SDL_Texture *texture = NULL;
  SDL_Surface *loadedImage = SDL_LoadBMP(file);
  if (loadedImage != NULL) {
    texture = SDL_CreateTextureFromSurface(ren, loadedImage);
    SDL_FreeSurface(loadedImage);
    if (texture == NULL) {
      printf("Error : CreateTextureFromSurface\n");
    }
  } else {
    printf("Error : LoadBMP\n");
  }
  return texture;
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int width,
                   int height) {
  SDL_Rect dst;
  dst.x = x;
  dst.y = y;
  dst.w = width;
  dst.h = height;
  SDL_RenderCopy(ren, tex, NULL, &dst);
}

// ############################################################## GESTION DE LA
// MAP #####################################################################

SDL_Texture **load_MAP_Textures(const char *tilefilename, SDL_Renderer *ren) {
  SDL_Texture *texture = NULL;
  SDL_Surface *loadedImage = SDL_LoadBMP(tilefilename);
  SDL_Texture **tabMAPTextures = (SDL_Texture **)malloc(
      (TILES_WIDTH * TILES_HEIGHT) * sizeof(SDL_Texture *));
  int i = 0;
  for (int top = 0; top < TILES_HEIGHT; top += 1) {
    for (int left = 0; left < TILES_WIDTH; left += 1) {
      SDL_Surface *tileSurf =
          SDL_CreateRGBSurface(0, TILE_SIZE, TILE_SIZE, 32, 0, 0, 0, 0);
      SDL_Rect tileRect;
      tileRect.x = (left * (TILE_SIZE + 1)) + 1;
      tileRect.y = (top * (TILE_SIZE + 1)) + 1;
      tileRect.w = TILE_SIZE;
      tileRect.h = TILE_SIZE;
      SDL_BlitSurface(loadedImage, &tileRect, tileSurf, NULL);
      SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, tileSurf);
      tabMAPTextures[i] = tex;
      SDL_FreeSurface(tileSurf);
      i++;
    }
  }
  SDL_FreeSurface(loadedImage);
  return tabMAPTextures;
}

void loadMap(const char *filename,
             int world_map[WORLD_MAP_HEIGHT][WORLD_MAP_WIDTH]) {
  char buf[300 * 3];
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Le fichier n'existe pas !\n");
  }
  int numLig = 0;
  int numCol = 0;
  while (fgets(buf, sizeof buf, fp) != NULL) {
    char *strToken = strtok(buf, " ");
    numCol = 0;
    while (strToken != NULL) {
      long num = strtol(strToken, NULL, 16);
      world_map[numLig][numCol] = num;
      strToken = strtok(NULL, " ");
      numCol++;
    }
    numLig++;
  }
  if (fclose(fp) == EOF) {
    printf("Erreur lors de la fermeture du flux\n");
  }
}

void loadBlockingMap(const char *filename,
                     char blocking_map[WORLD_MAP_HEIGHT][WORLD_MAP_WIDTH]) {
  char buf[300];
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Le fichier n'existe pas !\n");
  }
  int numLig = 0;
  int numCol = 0;
  while (fgets(buf, sizeof buf, fp) != NULL) {
    for (int j = 0; j < WORLD_MAP_WIDTH; j++) {
      blocking_map[numLig][j] = buf[j];
    }
    numLig++;
  }
  if (fclose(fp) == EOF) {
    printf("Erreur lors de la fermeture du flux\n");
  }
}

// ############################################################## GESTION
// MULTIMEDIA
// #####################################################################

void printText(int x, int y, char *texte, int wsize, int hsize,
               SDL_Renderer *ren) {
  SDL_Surface *surfaceMessage = TTF_RenderText_Solid(Sans, texte, Yellow);
  SDL_Texture *Message = SDL_CreateTextureFromSurface(ren, surfaceMessage);
  SDL_Rect Message_rect;
  Message_rect.x = x;
  Message_rect.y = y;
  Message_rect.w = wsize;
  Message_rect.h = hsize;
  SDL_RenderCopy(ren, Message, NULL, &Message_rect);
  SDL_FreeSurface(surfaceMessage);
  SDL_DestroyTexture(Message);
}

void playMusic(const char *filename) {
  int result = 0;
  int flags = MIX_INIT_MP3;

  if (SDL_Init(SDL_INIT_AUDIO) < 0) {
    printf("Failed to init SDL\n");
    exit(1);
  }

  if (flags != (result = Mix_Init(flags))) {
    printf("Could not initialize mixer (result: %d).\n", result);
    printf("Mix_Init: %s\n", Mix_GetError());
    exit(1);
  }

  Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640);
  Mix_Music *music = Mix_LoadMUS(filename);
  Mix_PlayMusic(music, 1);
}
