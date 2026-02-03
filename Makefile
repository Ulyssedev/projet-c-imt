CC = gcc
CFLAGS = -Wall -std=c11 `pkg-config --cflags sdl2 SDL2_ttf SDL2_mixer`
LDFLAGS = `pkg-config --libs sdl2 SDL2_ttf SDL2_mixer`
OBJ_NAME = zeldimt

all:
	$(CC) src/main.c $(CFLAGS) $(LDFLAGS) -o $(OBJ_NAME)