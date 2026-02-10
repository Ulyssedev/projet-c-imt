CC = gcc
CFLAGS = -Wall -std=c11 `pkg-config --cflags sdl2 SDL2_ttf SDL2_mixer`
LDFLAGS = `pkg-config --libs sdl2 SDL2_ttf SDL2_mixer`
OBJ_NAME = zeldimt

SRCS = src/main.c src/IOManager.c src/ressources.c src/game.c src/map.c src/enemy.c src/link.c src/character.c

all:
	$(CC) $(SRCS) $(CFLAGS) $(LDFLAGS) -o $(OBJ_NAME)