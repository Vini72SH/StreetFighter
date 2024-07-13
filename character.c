#include "character.h"

rectangle *createRectangle(float x, float y, ushort width, ushort height) {
    rectangle *newRec;

    newRec = malloc(sizeof(rectangle));
    if (!(newRec)) return NULL;

    newRec->x = x;
    newRec->y = y;
    newRec->width = width;
    newRec->height = height;

    return newRec;
};

character *createCharacter(uint hp, ushort x, ushort y, ushort max_x, ushort max_y) {
    character *newCharacter;

    if ((x - CHAR_WIDTH/2 < 0) || (x + CHAR_WIDTH/2 > max_x) || (y - CHAR_HEIGHT/2 < 0) || (y + CHAR_HEIGHT/2 > max_y)) return NULL;

    newCharacter = malloc(sizeof(character));
    if (!(newCharacter)) return NULL;

    newCharacter->hp = hp;
    newCharacter->x = x;
    newCharacter->y = y;
    newCharacter->air_speed = 0;
    newCharacter->state = IDLE;
    newCharacter->hurtbox = createRectangle(x, y, CHAR_WIDTH, CHAR_HEIGHT);
    newCharacter->control = createJoy();

    return newCharacter;
};

void characterUp(character *chara) {
    if (!(chara)) return;

    if (chara->state == DOWN) {
        chara->hurtbox->width = CHAR_WIDTH;
        chara->hurtbox->height = CHAR_HEIGHT;
    }
};

void characterDown(character *chara) {
    if (!(chara)) return;

    chara->hurtbox->width = CHAR_DOWN_WIDTH;
    chara->hurtbox->height = CHAR_DOWN_HEIGHT;
};

void characterMove(character *chara, short steps, ushort trajectory, ushort max_x, ushort max_y) {
    if (trajectory == LEFT) {
        if ((chara->x - steps*SPEED) - chara->hurtbox->width/2 >= 0) {
            chara->x = chara->x - steps * SPEED;
            chara->hurtbox->x = chara->x;
        }
    }

    if (trajectory == RIGHT) {
        if ((chara->x + steps*SPEED) + chara->hurtbox->width/2 <= max_x) { 
            chara->x = chara->x + steps * SPEED;
            chara->hurtbox->x = chara->x;
        }
    }

    if (trajectory == UP) {
        if ((chara->y - steps * chara->air_speed * JUMP_CONST) - chara->hurtbox->height/2 >= 0) {
            chara->y = chara->y - steps * chara->air_speed * JUMP_CONST;
            chara->hurtbox->y = chara->y;
        }
    }

    if (trajectory == DOWN) {
        if ((chara->y + steps*SPEED) + chara->hurtbox->height/2 <= max_y) {
            chara->y = chara->y + steps * SPEED;
            chara->hurtbox->y = chara->y;
        }
    }
};

void destroyRectangle(rectangle *rect) {
    free(rect);
};

void destroyCharacter(character *chara) {
    destroyRectangle(chara->hurtbox);
    destroyJoy(chara->control);
    free(chara);
};