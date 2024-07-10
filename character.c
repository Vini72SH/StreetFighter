#include "character.h"

character *createCharacter(uint hp, ushort side, ushort x, ushort y, ushort max_x, ushort max_y) {
    character *newCharacter;

    if ((x - side/2 < 0) || (x + side/2 > max_x) || (y - side/2 < 0) || (y + side/2 > max_y)) return NULL;

    newCharacter = malloc(sizeof(character));
    if (!(newCharacter)) return NULL;

    newCharacter->hp = hp;
    newCharacter->side = side;
    newCharacter->x = x;
    newCharacter->y = y;
    newCharacter->control = createJoy();

    return newCharacter;
};

void characterMove(character *chara, short steps, ushort trajectory, ushort max_x, ushort max_y) {
    if (trajectory == LEFT) {
        if ((chara->x - steps*SPEED) - chara->side/2 >= 0) chara->x = chara->x - steps * SPEED;
    }

    if (trajectory == RIGHT) {
        if ((chara->x + steps*SPEED) + chara->side/2 <= max_x) chara->x = chara->x + steps * SPEED;
    }

    if (trajectory == UP) {
        if ((chara->y - steps*SPEED) - chara->side/2 >= 0) chara->y = chara->y - steps * SPEED;
    }

    if (trajectory == DOWN) {
        if ((chara->y + steps*SPEED) + chara->side/2 <= max_y) chara->y = chara->y + steps * SPEED;
    }
};

void destroyCharacter(character *chara) {
    destroyJoy(chara->control);
    free(chara);
};