#include <stdio.h>
#include "game.h"
#include "character.h"

/* Retorna 1 se os personagens colidirem de alguma forma. */
uchar check_collision (character *p1, character *p2) {

    if ((((p1->hurtbox->y + p1->hurtbox->height/2 >= p2->hurtbox->y - p2->hurtbox->height/2) && (p2->hurtbox->y - p2->hurtbox->height/2 >= p1->hurtbox->y - p1->hurtbox->height/2)) ||
		 ((p2->hurtbox->y + p2->hurtbox->height/2 >= p1->hurtbox->y - p1->hurtbox->height/2) && (p1->hurtbox->y - p1->hurtbox->height/2 >= p2->hurtbox->y - p2->hurtbox->height/2))) &&
		(((p1->hurtbox->x + p1->hurtbox->width/2 >= p2->hurtbox->x - p2->hurtbox->width/2) && (p2->hurtbox->x - p2->hurtbox->width/2 >= p1->hurtbox->x - p1->hurtbox->width/2)) || 	
		 ((p2->hurtbox->x + p2->hurtbox->width/2 >= p1->hurtbox->x - p1->hurtbox->width/2) && (p1->hurtbox->x - p1->hurtbox->width/2 >= p2->hurtbox->x - p2->hurtbox->width/2)))) {
        return 1;
    }

    return 0;
};

/* Retorna 1 se a Hitbox de p1 atinge a Hurtbox de p2. */
uchar check_hit (character *p1, character *p2) {
    
    if ((((p1->hitbox->y + p1->hitbox->height/2 >= p2->hurtbox->y - p2->hurtbox->height/2 + CONST_HHURT) && (p2->hurtbox->y - p2->hurtbox->height/2 + CONST_HHURT >= p1->hitbox->y - p1->hitbox->height/2)) ||
		 ((p2->hurtbox->y + p2->hurtbox->height/2 - CONST_HHURT >= p1->hitbox->y - p1->hitbox->height/2) && (p1->hitbox->y - p1->hitbox->height/2 >= p2->hurtbox->y - p2->hurtbox->height/2 + CONST_HHURT))) &&
		(((p1->hitbox->x + p1->hitbox->width/2 >= p2->hurtbox->x - p2->hurtbox->width/2 + CONST_WHURT) && (p2->hurtbox->x - p2->hurtbox->width/2 + CONST_WHURT >= p1->hitbox->x - p1->hitbox->width/2)) || 	
		 ((p2->hurtbox->x + p2->hurtbox->width/2 - CONST_WHURT >= p1->hitbox->x - p1->hitbox->width/2) && (p1->hitbox->x - p1->hitbox->width/2 >= p2->hurtbox->x - p2->hurtbox->width/2 + CONST_WHURT)))) {
        return 1;
    }

    return 0;
};

/* Verifica as ações dos personagens, atualizando seus estados e posições. */
/* Salto: Personagem não pode estar abaixado, no ar, atacando, bloqueando ou sofrendo dano. */
/* Andar: Personangem não pode estar abaixado, no ar, atacando, bloqueando ou sofrendo dano. */
/* Abaixar: Personagem não pode estar no ar, atacando, bloqueando ou sofrendo dano. */
void charactersMovement (ALLEGRO_EVENT event, character *player1, character *player2) {
    if (event.keyboard.keycode == ALLEGRO_KEY_W) {
        joystick_up(player1->control);   
        if ((event.type == ALLEGRO_EVENT_KEY_DOWN) && (player1->state != DOWN) && (player1->state != AIR) && (player1->state != ATTACK) && (player1->state != HURT) && (player1->state != BLOCK)) {
            player1->state = AIR;
            player1->air_speed = JUMP_SPEED;
            player1->current_frame = JUMP0;
            player1->frame_delay = AIR_DELAY;
        }
    }
    if (event.keyboard.keycode == ALLEGRO_KEY_A) {
        joystick_left(player1->control);
        if ((player1->state != DOWN) && (player1->state != AIR) && (player1->state != ATTACK) && (player1->state != HURT) && (player1->state != BLOCK)) {
            player1->state = WALK;
            player1->frame_delay = WALK_DELAY;
        }
    }
    if (event.keyboard.keycode == ALLEGRO_KEY_D) {
        joystick_right(player1->control);
        if ((player1->state != DOWN) && (player1->state != AIR) && (player1->state != ATTACK) && (player1->state != HURT) && (player1->state != BLOCK)) {
            player1->state = WALK;
            player1->frame_delay = WALK_DELAY;
        }
    }
    if (event.keyboard.keycode == ALLEGRO_KEY_S) {
        joystick_down(player1->control);
        if ((player1->state != AIR) && (player1->state != ATTACK) && (player1->state != HURT) && (player1->state != BLOCK)) {
            player1->state = DOWN;
            player1->current_frame = DOWN0;
            characterDown(player1);
        }
    }
    if (event.keyboard.keycode == ALLEGRO_KEY_R) {
        if (!(player1->block) && (player1->state != AIR) && (player1->state != ATTACK) && (player1->state != HURT) && (event.type == ALLEGRO_EVENT_KEY_DOWN)) {
            player1->block = 1;
            player1->previous_state = player1->state;
            player1->previous_frame = player1->current_frame;
            player1->state = BLOCK;
            player1->current_frame = (player1->previous_state != DOWN)? STANDBLOCK : DOWNBLOCK;
        } else if ((player1->block) && (event.type == ALLEGRO_EVENT_KEY_UP)) {
            player1->block = 0;
            player1->state = player1->previous_state;
            player1->current_frame = player1->previous_frame;
        }
    }
    
    if (event.keyboard.keycode == ALLEGRO_KEY_UP) {
        joystick_up(player2->control);
        if ((event.type == ALLEGRO_EVENT_KEY_DOWN) && (player2->state != DOWN) && (player2->state != AIR) && (player2->state != ATTACK) && (player2->state != HURT) && (player2->state != BLOCK)) {
            player2->state = AIR;
            player2->air_speed = JUMP_SPEED;
            player2->current_frame = JUMP0;
            player2->frame_delay = AIR_DELAY;
        }
    }
    if (event.keyboard.keycode == ALLEGRO_KEY_LEFT) {
        joystick_left(player2->control);
        if ((player2->state != DOWN) && (player2->state != AIR) && (player2->state != ATTACK) && (player2->state != HURT) && (player2->state != BLOCK)) {
            player2->state = WALK;
            player2->frame_delay = WALK_DELAY;
        }
    }
    if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
        joystick_right(player2->control);
        if ((player2->state != DOWN) && (player2->state != AIR) && (player2->state != ATTACK) && (player2->state != HURT) && (player2->state != BLOCK)) {
            player2->state = WALK;
            player2->frame_delay = WALK_DELAY;
        }
    }
    if (event.keyboard.keycode == ALLEGRO_KEY_DOWN) {
        joystick_down(player2->control);
        if ((player2->state != AIR) && (player2->state != ATTACK) && (player2->state != HURT) && (player2->state != BLOCK)) {
            player2->state = DOWN;
            player2->current_frame = DOWN0;
            characterDown(player2);
        }
    }
    if (event.keyboard.keycode == ALLEGRO_KEY_PAD_4) {
        if (!(player2->block) && (player2->state != AIR) && (player2->state != ATTACK) && (player2->state != HURT) && (event.type == ALLEGRO_EVENT_KEY_DOWN)) {
            player2->block = 1;
            player2->previous_state = player2->state;
            player2->previous_frame = player2->current_frame;
            player2->state = BLOCK;
            player2->current_frame = (player2->previous_state != DOWN)? STANDBLOCK : DOWNBLOCK;
        } else if ((player2->block) && (event.type == ALLEGRO_EVENT_KEY_UP)) {
            player2->block = 0;
            player2->state = player2->previous_state;
            player2->current_frame = player2->previous_frame;
        }
    }
};

/* Atualiza as posições e movimentos, verifica as colisões na movimentação. */
void update_position(character *player1, character *player2) {
    if((player1 == NULL)  || (player2 == NULL)) {
        return;
    }
    /* Se o personagem não está no ar, atacando, sofrendo dano e nenhum botão está pressionado, então ele está parado. */
    if ((player1->state != AIR) && !((player1->control->up) || (player1->control->down) || (player1->control->left) || 
        (player1->control->right)) && (player1->state != IDLE) && (player1->state != ATTACK) && (player1->state != HURT) && (player1->state != BLOCK)) {
        player1->state = IDLE;
        player1->current_frame = IDLE0;
        player1->frame_delay = FRAME_DELAY;
    }
    
    if (player1->state == AIR){
        characterMove(player1, 1, UP, WIDTH, MAX_Y);
        player1->air_speed -= GRAVITY;
        /* Se houver colisão, afasta os personagens com flush. */
        if (check_collision(player1, player2)) {
            player1->air_speed += GRAVITY;
            characterMove(player1, -1, UP, WIDTH, MAX_Y);
            characterFlush(player1, player2, WIDTH, MAX_Y);
        }
        /* Define o estado após colisão com o chão. */
        if ((player1->y + player1->hurtbox->height/2 >= MAX_Y) && (player1->state != IDLE)) {
            if ((player1->control->left) || (player1->control->right)) {
                player1->state = WALK;
                player1->current_frame = WALK0;
                player1->frame_delay = WALK_DELAY;
            } else {
                player1->state = IDLE;
                player1->current_frame = IDLE0;
                player1->frame_delay = FRAME_DELAY;
            }
            player1->air_speed = 0;
        }
    }

    if (player1->control->down) {
        if (check_collision(player1, player2)) characterMove(player1, -1, DOWN, WIDTH, MAX_Y);
    } else if (player1->state != BLOCK) {
        /* Levanta o personagem e verifica colisões. */
        characterUp(player1);
        if ((player1->state == IDLE) && ((player1->control->left) || (player1->control->right))) {
            player1->state = WALK;
            player1->current_frame = WALK0;
            player1->frame_delay = WALK_DELAY;
        }
        /* Impede que o personagem se levante em caso de colisão. */
        if (check_collision(player1, player2) && (player2->state == AIR)) {
            characterDown(player1);
            player1->state = DOWN;
            player1->current_frame = DOWN0;
        }
    }

    /* Personagem se move apenas quando não está abaixado, atacando ou sofrendo dano. */
    if ((player1->state != DOWN) && (player1->state != ATTACK) && (player1->state != HURT) && (player1->state != BLOCK)) {
        if (player1->control->left) {
            characterMove(player1, 1, LEFT, WIDTH, MAX_Y);
            if (check_collision(player1, player2)) characterMove(player1, -1, LEFT, WIDTH, MAX_Y);
        }
        if (player1->control->right) {
            characterMove(player1, 1, RIGHT, WIDTH, MAX_Y);
            if (check_collision(player1, player2)) characterMove(player1, -1, RIGHT, WIDTH, MAX_Y);
        }
        if ((player1->control->right) && (player1->control->left)) {
            characterMove(player1, 1, RIGHT, WIDTH, MAX_Y);
            if (check_collision(player1, player2)) characterMove(player1, -1, RIGHT, WIDTH, MAX_Y);
        }
    }
    
    /* Se o personagem não está no ar, atacando, sofrendo dano e nenhum botão está pressionado, então ele está parado. */
    if ((player2->state != AIR) && !((player2->control->up) || (player2->control->down) || (player2->control->left) || 
        (player2->control->right)) && (player2->state != IDLE) && (player2->state != ATTACK) && (player2->state != HURT) && (player2->state != BLOCK)) {
        player2->state = IDLE;
        player2->current_frame = IDLE0;
        player2->frame_delay = FRAME_DELAY;
    }
    if (player2->state == AIR){
        characterMove(player2, 1, UP, WIDTH, MAX_Y);
        player2->air_speed -= GRAVITY;
        /* Se houver colisão, afasta os personagens com flush. */
        if (check_collision(player2, player1)) {
            player2->air_speed += GRAVITY;
            characterMove(player2, -1, UP, WIDTH, MAX_Y);
            characterFlush(player2, player1, WIDTH, MAX_Y);
        }
        /* Define o estado após colisão com o chão. */
        if ((player2->y + player2->hurtbox->height/2 >= MAX_Y) && (player2->state != IDLE)) {
            if ((player2->control->left) || (player2->control->right)) {
                player2->state = WALK;
                player2->current_frame = WALK0;
                player2->frame_delay = WALK_DELAY;
            } else {
                player2->state = IDLE;
                player2->current_frame = IDLE0;
                player2->frame_delay = FRAME_DELAY;
            }
            player2->air_speed = 0;
        }
    }

    if (player2->control->down) {
        if (check_collision(player2, player1)) characterMove(player2, -1, DOWN, WIDTH, MAX_Y);
    } else if (player2->state != BLOCK) {
        /* Levanta o personagem e verifica colisões. */
        characterUp(player2);
        if ((player2->state == IDLE) && ((player2->control->left) || (player2->control->right))) {
            player2->state = WALK;
            player2->current_frame = WALK0;
            player2->frame_delay = WALK_DELAY;
        }
        /* Impede que o personagem se levante em caso de colisão. */
        if (check_collision(player2, player1) && (player1->state == AIR)) {
            characterDown(player2);
            player2->state = DOWN;
            player2->current_frame = DOWN0;
        }
    }

    /* Personagem se move apenas quando não está abaixado, atacando ou sofrendo dano. */
    if ((player2->state != DOWN) && (player2->state != ATTACK) && (player2->state != HURT) && (player2->state != BLOCK)) {
        if (player2->control->left) {
            characterMove(player2, 1, LEFT, WIDTH, MAX_Y);
            if (check_collision(player2, player1)) characterMove(player2, -1, LEFT, WIDTH, MAX_Y);
        }
        if (player2->control->right) {
            characterMove(player2, 1, RIGHT, WIDTH, MAX_Y);
            if (check_collision(player2, player1)) characterMove(player2, -1, RIGHT, WIDTH, MAX_Y);
        }
        if ((player2->control->right) && (player2->control->left)) {
            characterMove(player2, 1, LEFT, WIDTH, MAX_Y);
            if (check_collision(player2, player1)) characterMove(player2, -1, LEFT, WIDTH, MAX_Y);
        }
    }

    invertDirections(player1, player2);
    invertDirections(player2, player1);
};

/* Atualizações de estado para ataque. */
void charactersAttack(ALLEGRO_EVENT event, character *player1, character *player2) {
    if (event.keyboard.keycode == ALLEGRO_KEY_T) {
        if (player1->state == IDLE) {
            player1->state = ATTACK;
            player1->current_frame = LIGHT0;
            player1->frame_delay = LIGHT_DELAY;
        } 
    }
    if (event.keyboard.keycode == ALLEGRO_KEY_PAD_5) {
        if (player2->state == IDLE) {
            player2->state = ATTACK;
            player2->current_frame = LIGHT0;
            player2->frame_delay = LIGHT_DELAY;
        } 
    }
};

/* Verificação se o ataque acertou. */
/* Definição de estados para o personagem atingido. */
void checkAttack(character *player1, character *player2) {
    if (player1->state == ATTACK) {
        if (((player1->current_frame == LIGHT2) || (player1->current_frame == LIGHT3)) && check_hit(player1, player2)) {
            if (!(player1->hit) && (player2->state != HURT)) {
                player1->hit = true;
                if (player2->current_frame != STANDBLOCK) {
                    player2->hp -= 10;
                    player2->previous_state = player2->state;
                    player2->previous_frame = player2->current_frame;
                    player2->previous_delay = player2->frame_delay;
                    player2->state = HURT;
                    player2->frame_delay = HURT_DELAY;
                    if (player2->previous_state != DOWN) {
                        player2->current_frame = STANDHURT0;
                    }
                    resetChar(player2);
                }
            }
        }
    }
    if (player2->state == ATTACK) {
        if (((player2->current_frame == LIGHT2) || (player2->current_frame == LIGHT3)) && check_hit(player2, player1)) {
            if (!(player2->hit) && (player1->state != HURT)) {
                player2->hit = true;
                if (player1->current_frame != STANDBLOCK) {
                    player1->hp -= 10;
                    player1->previous_state = player1->state;
                    player1->previous_frame = player1->current_frame;
                    player1->previous_delay = player1->frame_delay;
                    player1->state = HURT;
                    player1->frame_delay = HURT_DELAY;
                    if (player1->previous_state != DOWN) {
                        player1->current_frame = STANDHURT0;
                    }
                    resetChar(player1);
                }
            }
        }
    }
};

int check_game(character *player1, character *player2) {
    if ((player1->hp > 0) && (player2->hp > 0))
        return 1;
    return 0;
};