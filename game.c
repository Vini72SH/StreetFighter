#include <stdio.h>
#include "game.h"

uchar check_collision (character *p1, character *p2) {

    if ((((p1->y + p1->hurtbox->height/2 >= p2->y - p2->hurtbox->height/2) && (p2->y - p2->hurtbox->height/2 >= p1->y - p1->hurtbox->height/2)) ||
		 ((p2->y + p2->hurtbox->height/2 >= p1->y - p1->hurtbox->height/2) && (p1->y - p1->hurtbox->height/2 >= p2->y - p2->hurtbox->height/2))) &&
		(((p1->x + p1->hurtbox->width/2 >= p2->x - p2->hurtbox->width/2) && (p2->x - p2->hurtbox->width/2 >= p1->x - p1->hurtbox->width/2)) || 	
		 ((p2->x + p2->hurtbox->width/2 >= p1->x - p1->hurtbox->width/2) && (p1->x - p1->hurtbox->width/2 >= p2->x - p2->hurtbox->width/2)))) {
        return 1;
    }

    return 0;
};

void update_position(character *player1, character *player2) {
    if ((player1->state != AIR) && !((player1->control->up) || (player1->control->down) ||
                                     (player1->control->left) || (player1->control->right))) {
        player1->state = IDLE;
    }

    if (player1->state == AIR){
        characterMove(player1, 1, UP, WIDTH, MAX_Y);
        player1->air_speed -= GRAVITY;
        if (check_collision(player1, player2)) {
            player1->air_speed += GRAVITY;
            characterMove(player1, -1, UP, WIDTH, MAX_Y);
        }
        if (player1->y + player1->hurtbox->height/2 > MAX_Y) {
            player1->state = IDLE;
        }
    }

    if (player1->control->down) {
        if (check_collision(player1, player2)) characterMove(player1, -1, DOWN, WIDTH, MAX_Y);
    }
    if ((player1->control->left) && (player1->state != DOWN)) {
        characterMove(player1, 1, LEFT, WIDTH, MAX_Y);
        if (check_collision(player1, player2)) characterMove(player1, -1, LEFT, WIDTH, MAX_Y);
    }
    if ((player1->control->right) && (player1->state != DOWN)) {
        characterMove(player1, 1, RIGHT, WIDTH, MAX_Y);
        if (check_collision(player1, player2)) characterMove(player1, -1, RIGHT, WIDTH, MAX_Y);
    }

    if ((player2->state != AIR) && !((player2->control->up) || (player2->control->down) ||
                                     (player2->control->left) || (player2->control->right))) {
        player2->state = IDLE;
    }

    if (player2->state == AIR){
        characterMove(player2, 1, UP, WIDTH, MAX_Y);
        player2->air_speed -= GRAVITY;
        if (check_collision(player2, player1)) {
            player2->air_speed += GRAVITY;
            characterMove(player2, -1, UP, WIDTH, MAX_Y);
        }
        if (player2->y + player2->hurtbox->height/2 > MAX_Y) {
            player2->state = IDLE;
        }
    }

    if (player2->control->down) {
        if (check_collision(player2, player1)) characterMove(player2, -1, DOWN, WIDTH, MAX_Y);
    }
    if ((player2->control->left) && (player2->state != DOWN)) {
        characterMove(player2, 1, LEFT, WIDTH, MAX_Y);
        if (check_collision(player2, player1)) characterMove(player2, -1, LEFT, WIDTH, MAX_Y);
    }
    if ((player2->control->right) && (player2->state != DOWN)) {
        characterMove(player2, 1, RIGHT, WIDTH, HEIGHT - 50);
        if (check_collision(player2, player1)) characterMove(player2, -1, RIGHT, WIDTH, MAX_Y);
    }

};

void charactersMovement (ALLEGRO_EVENT event, character *player1, character *player2) {
    if (event.keyboard.keycode == ALLEGRO_KEY_W) {
        joystick_up(player1->control);
        if ((event.type == ALLEGRO_EVENT_KEY_DOWN) && (player1->state != DOWN) && (player1->state != AIR)) {
            player1->state = AIR;
            player1->air_speed = JUMP_SPEED;
        }
    }
    if (event.keyboard.keycode == ALLEGRO_KEY_A) {
        joystick_left(player1->control);
        if ((player1->state != DOWN) && (player1->state != AIR)) {
            player1->state = WALK;
        }  
    }
    if (event.keyboard.keycode == ALLEGRO_KEY_D) {
        joystick_right(player1->control);
        if ((player1->state != DOWN) && (player1->state != AIR)) {
            player1->state = WALK;
        }
    } 
    if (event.keyboard.keycode == ALLEGRO_KEY_S) {
        joystick_down(player1->control);
        if (player1->state != AIR) {
            player1->state = DOWN;
        }
    }

    if (event.keyboard.keycode == ALLEGRO_KEY_UP) {
        joystick_up(player2->control);
        if ((event.type == ALLEGRO_EVENT_KEY_DOWN) && (player2->state != DOWN) && (player2->state != AIR)) {
            player2->state = AIR;
            player2->air_speed = JUMP_SPEED;
        }
    }
    if (event.keyboard.keycode == ALLEGRO_KEY_LEFT) {
        joystick_left(player2->control);
        if ((player2->state != DOWN) && (player2->state != AIR)) {
            player2->state = WALK;
        }
    }
    if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
        joystick_right(player2->control);
        if ((player2->state != DOWN) && (player2->state != AIR)) {
            player2->state = WALK;
        }
    }
    if (event.keyboard.keycode == ALLEGRO_KEY_DOWN) {
        joystick_down(player2->control);
        if (player2->state != AIR) {
            player2->state = DOWN;
        }
    }
};