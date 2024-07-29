#ifndef __GAME__
#define __GAME__

#include "character.h"

#define WIDTH 1320
#define HEIGHT 680
#define MAX_Y 620
#define GRAVITY 1

void charactersMovement(ALLEGRO_EVENT event, character *player1, character *player2);
void update_position(character *player1, character *player2);
void charactersAttack(ALLEGRO_EVENT event, character *player1, character *player2);
void checkAttack(character *player1, character *player2);
int check_game(character *player1, character *player2);

#endif