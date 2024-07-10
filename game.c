#include "game.h"

uchar check_collision (character *element_first, character *element_second) {

    if ((((element_first->y+element_first->side/2 >= element_second->y-element_second->side/2) && (element_second->y-element_second->side/2 >= element_first->y-element_first->side/2)) ||			
		((element_second->y+element_second->side/2 >= element_first->y-element_first->side/2) && (element_first->y-element_first->side/2 >= element_second->y-element_second->side/2))) && 	
		(((element_first->x+element_first->side/2 >= element_second->x-element_second->side/2) && (element_second->x-element_second->side/2 >= element_first->x-element_first->side/2)) || 	
		((element_second->x+element_second->side/2 >= element_first->x-element_first->side/2) && (element_first->x-element_first->side/2 >= element_second->x-element_second->side/2)))) {
        return 1;
    }

    return 0;
};

void update_position(character *player1, character *player2) {
    if (player1->control->up) {
        characterMove(player1, 1, UP, WIDTH, HEIGHT - 50);
        if (check_collision(player1, player2)) characterMove(player1, -1, UP, WIDTH, HEIGHT - 50);
    }
    if (player1->control->down) {
        characterMove(player1, 1, DOWN, WIDTH, HEIGHT - 50);
        if (check_collision(player1, player2)) characterMove(player1, -1, DOWN, WIDTH, HEIGHT - 50);
    }
    if (player1->control->left) {
        characterMove(player1, 1, LEFT, WIDTH, HEIGHT - 50);
        if (check_collision(player1, player2)) characterMove(player1, -1, LEFT, WIDTH, HEIGHT - 50);
    }
    if (player1->control->right) {
        characterMove(player1, 1, RIGHT, WIDTH, HEIGHT - 50);
        if (check_collision(player1, player2)) characterMove(player1, -1, RIGHT, WIDTH, HEIGHT - 50);
    }

    if (player2->control->up) {
        characterMove(player2, 1, UP, WIDTH, HEIGHT - 50);
        if (check_collision(player2, player1)) characterMove(player2, -1, UP, WIDTH, HEIGHT - 50);
    }
    if (player2->control->down) {
        characterMove(player2, 1, DOWN, WIDTH, HEIGHT - 50);
        if (check_collision(player2, player1)) characterMove(player2, -1, DOWN, WIDTH, HEIGHT - 50);
    }
    if (player2->control->left) {
        characterMove(player2, 1, LEFT, WIDTH, HEIGHT - 50);
        if (check_collision(player2, player1)) characterMove(player2, -1, LEFT, WIDTH, HEIGHT - 50);
    }
    if (player2->control->right) {
        characterMove(player2, 1, RIGHT, WIDTH, HEIGHT - 50);
        if (check_collision(player2, player1)) characterMove(player2, -1, RIGHT, WIDTH, HEIGHT - 50);
    }
};

void charactersMovement (ALLEGRO_EVENT event, character *player1, character *player2) {
    if (event.keyboard.keycode == ALLEGRO_KEY_W) {
        joystick_up(player1->control);      
    }
    if (event.keyboard.keycode == ALLEGRO_KEY_S) {
        joystick_down(player1->control);      
    }
    if (event.keyboard.keycode == ALLEGRO_KEY_A) {
        joystick_left(player1->control);      
    }
    if (event.keyboard.keycode == ALLEGRO_KEY_D) {
        joystick_right(player1->control);      
    }

    if (event.keyboard.keycode == ALLEGRO_KEY_UP) {
        joystick_up(player2->control);      
    }
    if (event.keyboard.keycode == ALLEGRO_KEY_DOWN) {
        joystick_down(player2->control);      
    }
    if (event.keyboard.keycode == ALLEGRO_KEY_LEFT) {
        joystick_left(player2->control);      
    }
    if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
        joystick_right(player2->control);      
    }
};