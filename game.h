#ifndef __GAME__
#define __GAME__

#include "character.h"

#define WIDTH 1320
#define HEIGHT 680
#define GRAVITY 2

void update_position (character *player1, character *player2);
void charactersMovement (ALLEGRO_EVENT event, character *player1, character *player2);

#endif