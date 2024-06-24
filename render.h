#ifndef RENDER_H
#define RENDER_H

#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

#define CHARACTERS 4
#define IMAGES 4
#define WIDTH 1320
#define HEIGHT 680
#define FRAMERATE 30

#define START 0
#define MENU 1
#define SELECTION 2
#define GAME 3

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
    ALLEGRO_DISPLAY *display;
    ALLEGRO_BITMAP *background[IMAGES];
    ALLEGRO_BITMAP *chars[CHARACTERS];
}Screen_Render;

Screen_Render *startGame();
Figure *createFigure(int dx, int dy, int op, int max_op, const char *filename);
void fade_out(ALLEGRO_DISPLAY *display, ALLEGRO_BITMAP *current_bitmap, float speed);
void fade_in(ALLEGRO_DISPLAY *display, ALLEGRO_BITMAP *next_bitmap, float speed);
void drawStart(Screen_Render *render, Figure *arrow);
void startScreen(Screen_Render *render, Figure *arrow, ALLEGRO_EVENT event, int *i);
void drawSelection(Screen_Render *render, Figure *s1, Figure *s2, int *i);
void selectionScreen(Screen_Render *render, Figure *s1, Figure *s2, ALLEGRO_EVENT event, int *i);
void deleteFigure(Figure *figure);
void endGame(Screen_Render *screen);

#endif