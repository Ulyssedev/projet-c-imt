#ifndef IOMANAGER_H_
#define IOMANAGER_H_

#include "ressources.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>

// ########################################################################
// Définition des constantes

// ########################################################################
// Définition des variables globales

// #########################################################################
// FONCTIONS GENERALES ############################################

/**
 * Initialise la SDL
 */
void initSDL();

/**
 * Créé une fenêtre SDL
 * @param winName Nom de la fenêtre
 * @param winWidth Largeur de la fenêtre
 * @param winHeigth Hauteur de la fenêtre
 */
SDL_Window *createWindow(char winName[], int winWidth, int winHeigth);

/**
 * Créé un renderer
 * @param win La fenêtre SDL
 */
SDL_Renderer *createRenderer(SDL_Window *win);

/**
 * Met à jour l'affichage de l'écran
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 */
void updateDisplay(SDL_Renderer *ren);

/* Vide le rendu graphique
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 */
void clearRenderer(SDL_Renderer *ren);

/* Libère la mémoire de la fenêtre et du renderer
 * @param win Un pointeur sur une structure contenant la fenêtre
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 */
void QuitSDL(SDL_Window *win, SDL_Renderer *ren);

/**
 * Charge une texture dans le rendu graphique
 * @param file Le chemin vers la texture
 * @param ren Un pointeur vers la structure contenant le rendu
 */
SDL_Texture *loadTexture(const char *file, SDL_Renderer *ren);

/**
 * Met la texture dans le rendu graphique à une position (x,y) dans le repère
 * écran avec une taille de size
 * @param tex La texture à mettre dans le rendu
 * @param ren Un pointeur vers la structure contenant le rendu
 * @param x La coordonnée x dans le repère écran
 * @param y La coordonnée y dans le repère écran
 * @param size La taille de la texture sur l'écran
 */
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int width,
                   int height);

// ############################################################## GESTION DE LA
// MAP #####################################################################

/* Charge les texture de la MAP se trouvant dans le fichier tilefilename. Les
 * numéros de tiles vont de 0 à 159.
 * @param tilefilename le nom du fichier image contenant toutes les tiles du
 * monde de Zelda
 * @param ren un pointeur vers une structure qui contient l'état du rendu
 * graphique
 */
SDL_Texture **load_MAP_Textures(const char *tilefilename, SDL_Renderer *ren);

/* Charge la Map sous la forme d'un tableau 2D d'entier où chaque case contient
 * le numéro de Tile à afficher. Les numéros de tiles vont de 0 à 159.
 * @param filename le nom du fichier contenant la map
 * @param world_map "retourne" un tableau d'entiers contenant les numéros de
 * Tile de la map
 */
void loadMap(const char *filename,
             int world_map[WORLD_MAP_HEIGHT][WORLD_MAP_WIDTH]);

/* Charge la MAP de blocage sous la forme d'un tableau de caractères où chaque
 * case spécifie si elle peut être traversée ('.') ou si elle ne peut pas l'être
 * ('X')
 * @param filename le nom du fichier contenant zones de blocage ('X') dans la
 * map
 * @param blocking_map "retourne" un tableau de caractères contenant des 'X' où
 * Link ne peut pas avancer et des '.' où le chemin est libre.
 */
void loadBlockingMap(const char *filename,
                     char blocking_map[WORLD_MAP_HEIGHT][WORLD_MAP_WIDTH]);

// ############################################################## GESTION
// MULTIMEDIA
// #####################################################################

/**
 * Met dans le rendu graphique un texte à la position (x,y) dans le repère écran
 * avec une taille de (hsize x wsize)
 * @param x La coordonnée x dans le repère écran
 * @param y La coordonnée y dans le repère écran
 * @param wsize La largeur du message sur l'écran
 * @param hsize La hauteur du message sur l'écran
 * @param ren un pointeur vers une structure qui contient l'état du rendu
 * graphique
 */
void printText(int x, int y, char *texte, int wsize, int hsize,
               SDL_Renderer *ren);

/**
 * Joue la musique MP3 qui se trouve dans le fichier filename
 * @param filename Le nom du fichier MP3
 */
void playMusic(const char *filename);

#endif /* IOMANAGER_H */
