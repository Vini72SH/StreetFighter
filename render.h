#ifndef RENDER_H
#define RENDER_H

#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

#define IMAGES 3
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
}Screen_Render;

Screen_Render *startGame();
Figure *createFigure(int dx, int dy, int op, int max_op, const char *filename);
void gameRender(Screen_Render *screen);
void deleteFigure(Figure *figure);
void endGame(Screen_Render *screen);

#endif