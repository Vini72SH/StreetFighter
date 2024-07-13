#ifndef CHARACTER_H
#define CHARACTER_H

#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include "joystick.h"

#define CHAR_WIDTH 180
#define CHAR_HEIGHT 300
#define CHAR_DOWN_WIDTH 160
#define CHAR_DOWN_HEIGHT 180
#define SPEED 20
#define JUMP_CONST 1.7
#define JUMP_SPEED 20
#define IDLE 0
#define WALK 1
#define DOWN 2
#define AIR 3

typedef unsigned int uint;
typedef unsigned short ushort;

typedef struct rectangle{
    float x;
    float y;
    ushort width;
    ushort height;
}rectangle;

typedef struct character{
    uint hp;
    float x;
    float y;
    float air_speed;
    int state;
    joystick *control;
    rectangle *hurtbox;
}character;


rectangle *createRectangle(float x, float y, ushort width, ushort height);
character *createCharacter(uint hp, ushort x, ushort y, ushort max_x, ushort max_y);
void characterUp(character *chara);
void characterDown(character *chara);
void characterMove(character *chara, float steps, ushort trajectory, ushort max_x, ushort max_y);
void characterFlush (character *p1, character *p2, ushort max_x, ushort max_y);
void destroyRectangle(rectangle *rect);
void destroyCharacter(character *chara);

#endif