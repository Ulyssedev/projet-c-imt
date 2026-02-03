// Ce fichier va contenir les constantes et ressources globales du projet
#ifndef RESSOURCES_H_
#define RESSOURCES_H_

extern const int WORLD_MAP_HEIGHT;
extern const int WORLD_MAP_WIDTH;
extern const int TILE_SIZE;
extern const int TILES_WIDTH;
extern const int TILES_HEIGHT;

// Room/screen dimensions (in tiles)
#define ROOM_WIDTH_TILES 16
#define ROOM_HEIGHT_TILES 11

// World dimensions (in rooms)
#define WORLD_ROOMS_X 16
#define WORLD_ROOMS_Y 8

// Window dimensions (scaled)
#define SCALE_FACTOR 2
#define WINDOW_WIDTH (ROOM_WIDTH_TILES * 16 * SCALE_FACTOR)
#define WINDOW_HEIGHT (ROOM_HEIGHT_TILES * 16 * SCALE_FACTOR)

#endif /* RESSOURCES_H_ */
