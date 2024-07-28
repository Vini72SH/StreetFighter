#ifndef CHARACTER_H
#define CHARACTER_H

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include "joystick.h"

#define WIDTH 1320
#define HEIGHT 680
#define NUM_SPRITES 23
#define ID_RYU 0
#define ID_KEN 1
#define ID_SAGAT 2
#define ID_CHUNLI 3
#define CHAR_WIDTH 180
#define CHAR_HEIGHT 300
#define CHAR_DOWN_WIDTH 180
#define CHAR_DOWN_HEIGHT 200
#define CONST_WHURT 15
#define CONST_HHURT 20
#define START_HITBOX 10
#define HP 100
#define FRAME_DELAY 5
#define AIR_DELAY 9
#define WALK_DELAY 2
#define LIGHT_DELAY 3
#define SPEED 15
#define JUMP_CONST 1.8
#define JUMP_SPEED 20
#define RIGHT_DIR 0
#define LEFT_DIR 1
#define IDLE 0
#define WALK 1
#define DOWN 2
#define AIR 3

#define P1_X 150
#define P2_X WIDTH - 150
#define START_Y 620 - CHAR_HEIGHT/2

typedef unsigned int uint;
typedef unsigned short ushort;

typedef enum{
    IDLE0,
    IDLE1,
    IDLE2,
    IDLE3,
    IDLE4,
    DOWN0,
    WALK0,
    WALK1,
    WALK2,
    WALK3,
    WALK4,
    WALK5,
    JUMP0,
    JUMP1,
    JUMP2,
    JUMP3,
    JUMP4,
    LIGHT0,
    LIGHT1,
    LIGHT2,
    LIGHT3,
    LIGHT4,
    LIGHT5,
}CharacterFrame;

typedef struct rectangle{
    float x;
    float y;
    ushort width;
    ushort height;
}rectangle;

typedef struct character{
    ushort id;
    uint hp;
    float x;
    float y;
    float air_speed;
    short dir;
    int state;
    int frame_delay;
    int frame_counter;
    joystick *control;
    rectangle *hitbox;
    rectangle *hurtbox;
    rectangle *char_render;
    CharacterFrame current_frame;
    ALLEGRO_BITMAP **sprites;
}character;

void resetCounter(int *counter);
ALLEGRO_BITMAP **loadSprites(short id);
rectangle *createRectangle(float x, float y, ushort width, ushort height);
character *createCharacter(ushort x, ushort y, ushort max_x, ushort max_y, short op, short dir);
void characterUp(character *chara);
void characterDown(character *chara);
void characterMove(character *chara, float steps, ushort trajectory, ushort max_x, ushort max_y);
void characterFlush (character *p1, character *p2, ushort max_x, ushort max_y);
void updateAnimation(character *chara);
void invertDirections(character *p1, character *p2);
void deleteSprites(character *chara);
void destroyRectangle(rectangle *rect);
void destroyCharacter(character *chara);

#endif