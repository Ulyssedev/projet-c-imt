CC = gcc
CFLAGS = -Wall -std=c11 `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs`
OBJ_NAME = zeldimt

all:
	$(CC) src/main.c $(CFLAGS) $(LDFLAGS) -o $(OBJ_NAME)

clean:
	rm -f $(OBJ_NAME)
