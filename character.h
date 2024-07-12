#ifndef CHARACTER_H
#define CHARACTER_H

#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include "joystick.h"

#define SPEED 20
#define JUMP_SPEED 10
#define IDLE 0
#define WALK 1
#define DOWN 2
#define AIR 3

typedef unsigned int uint;
typedef unsigned short ushort;

typedef struct character{
    uint hp;
    ushort side;
    float x;
    float y;
    float air_speed;
    int state;
    joystick *control;
}character;

character *createCharacter(uint hp, ushort side, ushort x, ushort y, ushort max_x, ushort max_y);
void characterMove(character *chara, short steps, ushort trajectory, ushort max_x, ushort max_y);
void destroyCharacter(character *chara);

#endif