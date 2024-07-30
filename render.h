#ifndef RENDER_H
#define RENDER_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include "character.h"

#define CHARACTERS 4
#define IMAGES 14
#define WIDTH 1320
#define HEIGHT 680
#define MAX_Y 620
#define FRAMERATE 30

#define START 0
#define MENU 1
#define SELECTION 2
#define GAME 3
#define ENDGAME 4

#define COLOR_BLACK al_map_rgb(0, 0, 0)
#define COLOR_RED al_map_rgb(255, 0, 0)
#define COLOR_BLUE al_map_rgb(0, 0, 255)
#define COLOR_GREEN al_map_rgb(0, 255, 0)
#define COLOR_PURPLE al_map_rgb(139, 0, 139)
#define COLOR_ORANGE al_map_rgb(255, 128, 0)
#define COLOR_WHITE al_map_rgb(255, 255, 255)

typedef struct Figure{
    int dx, dy;
    int w, h;
    int op;
    int max_op;
    int move;
    bool itOk;
    ALLEGRO_BITMAP *image;
}Figure;


typedef struct Screen_Render{
    int currentBackground;
    int gameMode;
    ALLEGRO_TIMER *timer;
    ALLEGRO_EVENT_QUEUE *queue;
    ALLEGRO_FONT *font1;
    ALLEGRO_FONT *font2;
    ALLEGRO_FONT *font3;
    ALLEGRO_DISPLAY *display;
    ALLEGRO_BITMAP *icon;
    ALLEGRO_BITMAP *lifebar;
    ALLEGRO_BITMAP *background[IMAGES];
    ALLEGRO_BITMAP *chars[CHARACTERS];
}Screen_Render;

Screen_Render *startGame();
Figure *createFigure(int dx, int dy, int op, int max_op, const char *filename);
void fade_out(ALLEGRO_DISPLAY *display, ALLEGRO_BITMAP *current_bitmap, float speed);
void fade_in(ALLEGRO_DISPLAY *display, ALLEGRO_BITMAP *next_bitmap, float speed);
void drawStart(Screen_Render *render, Figure *arrow);
void startScreen(Screen_Render *render, Figure *arrow, ALLEGRO_EVENT event, int *i);
void drawSelection(Screen_Render *render, Figure *s1, Figure *s2, Figure *s3, int *i);
void selectionScreen(Screen_Render *render, Figure *s1, Figure *s2, Figure *s3, ALLEGRO_EVENT event, int *i, bool *load);
void drawGame(Screen_Render *render, character *p1, character *p2, int *i, int round, bool change);
void drawLifebars(Screen_Render *render, character *p1, character *p2);
void drawRound(Screen_Render *render, int round);
void countRound(Screen_Render *render, bool *change, int *cont, int round);
void endRound(Screen_Render *render, int *winP1, int *winP2, character *player1, character *player2, bool *change, int *round, int *cont);
void deleteFigure(Figure *figure);
void endGame(Screen_Render *screen);

#endif