#include "character.h"

/* Carrega os vetores de sprites. */
ALLEGRO_BITMAP **loadSprites(short id){
    ALLEGRO_BITMAP **sprites;

    /* Alocação de Memória. */
    sprites = malloc(NUM_SPRITES * sizeof(ALLEGRO_BITMAP *));
    if (!(sprites)) {
        fprintf(stderr, "Falha ao alocar memória para os bitmaps.\n");
        return NULL;
    }

    if (id == ID_RYU) {
        for (int i = 0; i < NUM_SPRITES; ++i) {
            char filename[50];
            sprintf(filename, "./characters/RyuSprites/RyuSprite%d.bmp", i + 1);
            sprites[i] = al_load_bitmap(filename);
            if (!sprites[i]) {
                fprintf(stderr, "Falha ao carregar a imagem: %s\n", filename);
                for (int j = 0; j < i; ++j) {
                    al_destroy_bitmap(sprites[j]);
                }
                free(sprites);
                return NULL;
            }
        }
        return sprites;
    }

    if (id == ID_KEN) {
        for (int i = 0; i < NUM_SPRITES; ++i) {
            char filename[50];
            sprintf(filename, "./characters/KenSprites/KenSprite%d.bmp", i + 1);
            sprites[i] = al_load_bitmap(filename);
            if (!sprites[i]) {
                fprintf(stderr, "Falha ao carregar a imagem: %s\n", filename);
                for (int j = 0; j < i; ++j) {
                    al_destroy_bitmap(sprites[j]);
                }
                free(sprites);
                return NULL;
            }
        }
        return sprites;
    }

    if (id == ID_SAGAT) {
        for (int i = 0; i < NUM_SPRITES; ++i) {
            char filename[50];
            sprintf(filename, "./characters/SagatSprites/SagatSprite%d.bmp", i + 1);
            sprites[i] = al_load_bitmap(filename);
            if (!sprites[i]) {
                fprintf(stderr, "Falha ao carregar a imagem: %s\n", filename);
                for (int j = 0; j < i; ++j) {
                    al_destroy_bitmap(sprites[j]);
                }
                free(sprites);
                return NULL;
            }
        }
        return sprites;
    }

        if (id == ID_CHUNLI) {
        for (int i = 0; i < NUM_SPRITES; ++i) {
            char filename[50];
            sprintf(filename, "./characters/ChunLiSprites/ChunLiSprite%d.bmp", i + 1);
            sprites[i] = al_load_bitmap(filename);
            if (!sprites[i]) {
                fprintf(stderr, "Falha ao carregar a imagem: %s\n", filename);
                for (int j = 0; j < i; ++j) {
                    al_destroy_bitmap(sprites[j]);
                }
                free(sprites);
                return NULL;
            }
        }
        return sprites;
    }

    free(sprites);

    return NULL;
};

/* Cria e retorna um retângulo. */
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

/* Cria o personagem dentro da área de jogo. */
character *createCharacter(ushort x, ushort y, ushort max_x, ushort max_y, short op, short dir) {
    character *newCharacter;
    
    if ((x - CHAR_WIDTH/2 < 0) || (x + CHAR_WIDTH/2 > max_x) || (y - CHAR_HEIGHT/2 < 0) || (y + CHAR_HEIGHT/2 > max_y)) return NULL;

    newCharacter = malloc(sizeof(character));
    if (!(newCharacter)) return NULL;

    newCharacter->id = op;
    newCharacter->hp = HP;
    newCharacter->x = x;
    newCharacter->y = y;
    newCharacter->air_speed = 0;
    newCharacter->dir = dir;
    newCharacter->hit = false;
    newCharacter->state = IDLE;
    newCharacter->block = 0;
    newCharacter->current_frame = IDLE0;
    newCharacter->frame_delay = FRAME_DELAY;
    newCharacter->frame_counter = 0;
    newCharacter->previous_state = IDLE;
    newCharacter->previous_frame = IDLE0;
    newCharacter->previous_delay = FRAME_DELAY;
    newCharacter->hitbox = createRectangle(x, y, START_HITBOX, START_HITBOX);
    newCharacter->hurtbox = createRectangle(x, y, CHAR_WIDTH, CHAR_HEIGHT);
    newCharacter->char_render = createRectangle(x, y, CHAR_WIDTH, CHAR_HEIGHT);
    newCharacter->control = createJoy();
    newCharacter->sprites = loadSprites(op);

    return newCharacter;
};

/* Levanta o personagem, redefinindo suas dimensões. */
void characterUp(character *chara) {
    if (!(chara)) return;

    /* Ajuste no Y. */
    if (chara->char_render->height != CHAR_HEIGHT) {
        chara->y = chara->y - (CHAR_HEIGHT/2 - CHAR_DOWN_HEIGHT/2);
        chara->hitbox->y = chara->y;
        chara->hurtbox->y = chara->y;
        chara->char_render->y = chara->y;
    }

    if (chara->state == DOWN) {
        chara->state = IDLE;
        chara->current_frame = IDLE0;
    }

    chara->hurtbox->width = CHAR_WIDTH;
    chara->hurtbox->height = CHAR_HEIGHT;
    chara->char_render->width = CHAR_WIDTH;
    chara->char_render->height = CHAR_HEIGHT;
    chara->char_render->x = chara->x;
    chara->char_render->y = chara->y;

    if ((chara->current_frame == DOWNHURT)) {
        chara->current_frame = STANDHURT;
    }

};

/* Abaixa o personagem, redefinindo suas dimensões. */
void characterDown(character *chara) {
    if (!(chara)) return;

    /* Ajuste no Y. */
    if (chara->char_render->height != CHAR_DOWN_HEIGHT) {
        chara->y = chara->y + (CHAR_HEIGHT/2 - CHAR_DOWN_HEIGHT/2);
        chara->hitbox->y = chara->y;
        chara->hurtbox->y = chara->y;
        chara->char_render->y = chara->y;
    }
    
    chara->hurtbox->width = CHAR_DOWN_WIDTH;
    chara->hurtbox->height = CHAR_DOWN_HEIGHT;
    chara->char_render->width = CHAR_DOWN_WIDTH;
    chara->char_render->height = CHAR_DOWN_HEIGHT;
};

/* Movimentação do personagem. */
void characterMove(character *chara, float steps, ushort trajectory, ushort max_x, ushort max_y) {
    if (trajectory == LEFT) {
        if ((chara->x - steps*SPEED) - chara->char_render->width/2 >= 0) {
            chara->x = chara->x - steps * SPEED;
            chara->hitbox->x = chara->hitbox->x - steps * SPEED;
            chara->hurtbox->x = chara->x;
            chara->char_render->x = chara->char_render->x - steps * SPEED;
        }
    }

    if (trajectory == RIGHT) {
        if ((chara->x + steps*SPEED) + chara->char_render->width/2 <= max_x) { 
            chara->x = chara->x + steps * SPEED;
            chara->hitbox->x = chara->hitbox->x + steps * SPEED;
            chara->hurtbox->x = chara->x;
            chara->char_render->x = chara->char_render->x + steps * SPEED;
        }
    }

    if (trajectory == UP) {
        chara->y = chara->y - steps * chara->air_speed * JUMP_CONST;
        chara->hitbox->y = chara->y;
        chara->hurtbox->y = chara->y;
        chara->char_render->y = chara->y;
    }

    if (trajectory == DOWN) {
        if ((chara->y + steps*SPEED) + chara->char_render->height/2 <= max_y) {
            chara->y = chara->y + steps * SPEED;
            chara->hitbox->y = chara->y;
            chara->hurtbox->y = chara->y;
            chara->char_render->y = chara->y;
        }
    }
};

/* Empurra o personagem para evitar colisões por cima. */
void characterFlush(character *p1, character *p2, ushort max_x, ushort max_y) {
    if (p1->x <= p2->x) {
        characterMove(p1, 1.5, LEFT, max_x, max_y);
        characterMove(p2, 1.5, RIGHT, max_x, max_y);
    } else {
        characterMove(p1, 1.5, RIGHT, max_x, max_y);
        characterMove(p2, 1.5, LEFT, max_x, max_y);
    }
};

/* Atualiza a animação, indo para o próximo sprite. */
void updateAnimation(character *chara) {
    bool change = false;
    chara->frame_counter++;
    if (chara->frame_counter >= chara->frame_delay) {
        chara->frame_counter = 0;
        change = true;
    }

    if (change) {
        if (chara->state == IDLE) {
            switch (chara->current_frame) {
                case IDLE0:
                    chara->current_frame = IDLE1;
                    break;
                case IDLE1:
                    chara->current_frame = IDLE2;
                    break;
                case IDLE2:
                    chara->current_frame = IDLE3;
                    break;
                case IDLE3:
                    chara->current_frame = IDLE4;
                    break;
                case IDLE4:
                    chara->current_frame = IDLE0;
                    break;
                default:
                    chara->current_frame = IDLE0;
                    break;
            }
        }
        if (chara->state == WALK) {
            switch (chara->current_frame) {
            case WALK0:
                chara->current_frame = WALK1;
                break;
            case WALK1:
                chara->current_frame = WALK2;
                break;
            case WALK2:
                chara->current_frame = WALK3;
                break;
            case WALK3:
                chara->current_frame = WALK4;
                break;
            case WALK4:
                chara->current_frame = WALK5;
                break;
            case WALK5:
                chara->current_frame = WALK0;
                break;
            default:
                chara->current_frame = WALK0;
                break;
            }
        }
        if (chara->state == DOWN) {
            chara->current_frame = DOWN0;
        }
        if (chara->state == AIR) {
            switch (chara->current_frame) {
            case JUMP0:
                chara->current_frame = JUMP1;
                break;
            case JUMP1:
                chara->current_frame = JUMP2;
                break;
            case JUMP2:
                chara->current_frame = JUMP3;
                break;
            case JUMP3:
                chara->current_frame = JUMP4;
                break;
            case JUMP4:
                chara->current_frame = JUMP4;
                break;
            default:
                chara->current_frame = IDLE0;
                break;
            }
        }
        if (chara->state == ATTACK) {
            switch (chara->current_frame) {
            case LIGHT0:
                chara->current_frame = LIGHT1;
                changeHitbox(chara);
                break;
            case LIGHT1:
                chara->current_frame = LIGHT2;
                changeHitbox(chara);
                break;
            case LIGHT2:
                chara->current_frame = LIGHT3;
                changeHitbox(chara);
                break;
            case LIGHT3:
                chara->current_frame = LIGHT4;
                changeHitbox(chara);
                break;
            case LIGHT4:
                chara->current_frame = LIGHT5;
                changeHitbox(chara);
                break;
            case LIGHT5:
                chara->current_frame = IDLE0;
                chara->state = IDLE;
                chara->frame_delay = FRAME_DELAY;
                break;
            case SLIGHT0:
                chara->current_frame = SLIGHT1;
                changeHitbox(chara);
                break;
            case SLIGHT1:
                chara->current_frame = SLIGHT2;
                changeHitbox(chara);
                break;
            case SLIGHT2:
                chara->current_frame = SLIGHT3;
                changeHitbox(chara);
                break;
            case SLIGHT3:
                chara->current_frame = IDLE0;
                chara->state = IDLE;
                chara->frame_delay = FRAME_DELAY;
                break;
            case HEAVY0:
                chara->current_frame = HEAVY1;
                changeHitbox(chara);
                break;
            case HEAVY1:
                chara->current_frame = HEAVY2;
                changeHitbox(chara);
                break;
            case HEAVY2:
                chara->current_frame = HEAVY3;
                changeHitbox(chara);
                break;
            case HEAVY3:
                chara->current_frame = HEAVY4;
                changeHitbox(chara);
                break;
            case HEAVY4:
                chara->current_frame = HEAVY5;
                changeHitbox(chara);
                break;
            case HEAVY5:
                chara->current_frame = IDLE0;
                chara->state = IDLE;
                chara->frame_delay = FRAME_DELAY;
                break;
            case SHEAVY0:
                chara->current_frame = SHEAVY1;
                changeHitbox(chara);
                break;
            case SHEAVY1:
                chara->current_frame = SHEAVY2;
                changeHitbox(chara);
                break;
            case SHEAVY2:
                chara->current_frame = SHEAVY3;
                changeHitbox(chara);
                break;
            case SHEAVY3:
                chara->current_frame = SHEAVY4;
                changeHitbox(chara);
                break;
            case SHEAVY4:
                chara->current_frame = SHEAVY5;
                changeHitbox(chara);
                break;
            case SHEAVY5:
                chara->current_frame = SHEAVY6;
                changeHitbox(chara);
                break;
           case SHEAVY6:
                chara->current_frame = SHEAVY7;
                changeHitbox(chara);
                break;
            case SHEAVY7:
                chara->current_frame = IDLE0;
                chara->state = IDLE;
                chara->frame_delay = FRAME_DELAY;
                break;
            case DLIGHT0:
                chara->current_frame = DLIGHT1;
                changeHitbox(chara);
                break;
            case DLIGHT1:
                chara->current_frame = DLIGHT2;
                changeHitbox(chara);
                break;
            case DLIGHT2:
                chara->current_frame = DLIGHT3;
                changeHitbox(chara);
                break;
            case DLIGHT3:
                chara->current_frame = DOWN0;
                chara->state = DOWN;
                chara->frame_delay = FRAME_DELAY;
                break;
            case DSLIGHT0:
                chara->current_frame = DSLIGHT1;
                changeHitbox(chara);
                break;
            case DSLIGHT1:
                chara->current_frame = DSLIGHT2;
                changeHitbox(chara);
                break;
            case DSLIGHT2:
                chara->current_frame = DSLIGHT3;
                changeHitbox(chara);
                break;
            case DSLIGHT3:
                chara->current_frame = DSLIGHT4;
                changeHitbox(chara);
                break;
            case DSLIGHT4:
                chara->current_frame = DSLIGHT5;
                changeHitbox(chara);
                break;
            case DSLIGHT5:
                chara->current_frame = DOWN0;
                chara->state = DOWN;
                chara->frame_delay = FRAME_DELAY;
                break;
            default:
                break;
            }
        }
        /* Retorno ao estado anterior. */
        if (chara->state == HURT) {
            if ((chara->previous_state != ATTACK) && (chara->previous_state != BLOCK)) {
                chara->state = chara->previous_state;
                chara->current_frame = chara->previous_frame;
                chara->frame_delay = chara->previous_delay;
            } else {
                if (chara->hurtbox->width == CHAR_WIDTH) {
                    chara->state = IDLE;
                    chara->current_frame = IDLE0;
                } else {
                    chara->state = DOWN;
                    chara->current_frame = DOWN0;
                }
                chara->frame_delay = FRAME_DELAY;
            }
        }
    }
};

/* Altera o sentido do personagem. */
void invertDirections(character *p1, character *p2) { 
    if (p1->state != ATTACK) {
        if (p1->x > p2->x) {
            p1->dir = LEFT_DIR;
        }else{
            p1->dir = RIGHT_DIR;
        }
    }
};

/* Altera a Hitbox baseado no ataque. */
void changeHitbox(character *player) {
    if (player->id == ID_RYU) {
        if ((player->current_frame == LIGHT1) || (player->current_frame == LIGHT4) || (player->current_frame == SLIGHT0) || (player->current_frame == SLIGHT3)) {
            player->hit = false;
            player->hitbox->x = player->x;
            player->hitbox->y = player->y - al_get_bitmap_height(player->sprites[player->current_frame])/3.5;
            player->hitbox->width = START_HITBOX;
            player->hitbox->height = START_HITBOX;
        }
        if ((player->current_frame == LIGHT2) || (player->current_frame == LIGHT3)) {
            player->hitbox->width = LIGHT_HITBOX;
            player->hitbox->height = LIGHT_HITBOX;
            player->hitbox->x = player->x + ((1 - 2*player->dir) * al_get_bitmap_width(player->sprites[player->current_frame])/1.7) + (1 - player->dir % 2) * 25;
        }
        if ((player->current_frame == SLIGHT1) || (player->current_frame == SLIGHT2)) {
            player->hitbox->width = LIGHT_HITBOX;
            player->hitbox->height = LIGHT_HITBOX;
            player->hitbox->y = player->y - al_get_bitmap_height(player->sprites[player->current_frame])/3.5;
            player->hitbox->x = player->x + ((1 - 2*player->dir) * al_get_bitmap_width(player->sprites[player->current_frame])/1.65) + (1 - player->dir % 2) * 1;
        }
        if ((player->current_frame == HEAVY1) || (player->current_frame == HEAVY4)) {
            player->hit = false;
            player->hitbox->x = player->x;
            player->hitbox->y = player->y + al_get_bitmap_height(player->sprites[player->current_frame])/5.6;
            player->hitbox->width = START_HITBOX;
            player->hitbox->height = START_HITBOX;
        }
        if ((player->current_frame == HEAVY2) || (player->current_frame == HEAVY3)) {
            player->hitbox->width = LIGHT_HITBOX;
            player->hitbox->height = LIGHT_HITBOX;
            player->hitbox->x = player->x + ((1 - 2*player->dir) * al_get_bitmap_width(player->sprites[player->current_frame])/1.6) + (1 - player->dir % 2) * (-24);
        }
        if ((player->current_frame == SHEAVY2) || (player->current_frame == SHEAVY5)) {
            player->hit = false;
            player->hitbox->x = player->x;
            player->hitbox->width = START_HITBOX;
            player->hitbox->height = START_HITBOX;
            player->hitbox->y = player->y - al_get_bitmap_height(player->sprites[player->current_frame])/2.3;
        }
        if ((player->current_frame == SHEAVY3) || (player->current_frame == SHEAVY4)) {
            player->hitbox->width = LIGHT_HITBOX;
            player->hitbox->height = LIGHT_HITBOX;
            player->hitbox->x = player->x + ((1 - 2*player->dir) * al_get_bitmap_width(player->sprites[player->current_frame])/1.8) + (1 - player->dir % 2) * (37);
        }
        if ((player->current_frame == DLIGHT1) || (player->current_frame == DLIGHT2)) {
            player->hitbox->width = LIGHT_HITBOX;
            player->hitbox->height = LIGHT_HITBOX;
            player->hitbox->y = player->y - al_get_bitmap_height(player->sprites[player->current_frame])/5;
            player->hitbox->x = player->x + ((1 - 2*player->dir) * al_get_bitmap_width(player->sprites[player->current_frame])/2) + (1 - player->dir % 2) * (15);
        }
        if (player->current_frame == DLIGHT3) {
            player->hit = false;
            player->hitbox->x = player->x;
            player->hitbox->y = player->y;
            player->hitbox->width = START_HITBOX;
            player->hitbox->height = START_HITBOX;
        }
        if ((player->current_frame == DSLIGHT1) || (player->current_frame == DSLIGHT4)) {
            player->hit = false;
            player->hitbox->x = player->x;
            player->hitbox->width = START_HITBOX;
            player->hitbox->height = START_HITBOX;
            player->hitbox->y = player->y - al_get_bitmap_height(player->sprites[player->current_frame])/4;
        }
        if ((player->current_frame == DSLIGHT2) || (player->current_frame == DSLIGHT3)) {
            player->hitbox->width = LIGHT_HITBOX;
            player->hitbox->height = LIGHT_HITBOX;
            player->hitbox->x = player->x + ((1 - 2*player->dir) * al_get_bitmap_width(player->sprites[player->current_frame])/1.9) + (1 - player->dir % 2) * (20);
        }
    }

    if (player->id == ID_KEN) {
        if ((player->current_frame == LIGHT1) || (player->current_frame == LIGHT4) || (player->current_frame == SLIGHT0) || (player->current_frame == SLIGHT3)) {
            player->hit = false;
            player->hitbox->x = player->x;
            player->hitbox->y = player->y - al_get_bitmap_height(player->sprites[player->current_frame])/3.7;
            player->hitbox->width = START_HITBOX;
            player->hitbox->height = START_HITBOX;
        }
        if ((player->current_frame == LIGHT2) || (player->current_frame == LIGHT3)) {
            player->hitbox->width = LIGHT_HITBOX;
            player->hitbox->height = LIGHT_HITBOX;
            player->hitbox->x = player->x + ((1 - 2*player->dir) * al_get_bitmap_width(player->sprites[player->current_frame])/1.7) + (1 - player->dir % 2) * 25;
        }
        if ((player->current_frame == SLIGHT1) || (player->current_frame == SLIGHT2)) {
            player->hitbox->width = LIGHT_HITBOX;
            player->hitbox->height = LIGHT_HITBOX;
            player->hitbox->y = player->y - al_get_bitmap_height(player->sprites[player->current_frame])/3.5;
            player->hitbox->x = player->x + ((1 - 2*player->dir) * al_get_bitmap_width(player->sprites[player->current_frame])/1.65) + (1 - player->dir % 2) * 1;
        }
        if ((player->current_frame == HEAVY1) || (player->current_frame == HEAVY4)) {
            player->hit = false;
            player->hitbox->x = player->x;
            player->hitbox->y = player->y + al_get_bitmap_height(player->sprites[player->current_frame])/5.6;
            player->hitbox->width = START_HITBOX;
            player->hitbox->height = START_HITBOX;
        }
        if ((player->current_frame == HEAVY2) || (player->current_frame == HEAVY3)) {
            player->hitbox->width = LIGHT_HITBOX;
            player->hitbox->height = LIGHT_HITBOX;
            player->hitbox->x = player->x + ((1 - 2*player->dir) * al_get_bitmap_width(player->sprites[player->current_frame])/1.6) + (1 - player->dir % 2) * (-24);
        }
        if ((player->current_frame == SHEAVY2) || (player->current_frame == SHEAVY5)) {
            player->hit = false;
            player->hitbox->x = player->x;
            player->hitbox->width = START_HITBOX;
            player->hitbox->height = START_HITBOX;
            player->hitbox->y = player->y - al_get_bitmap_height(player->sprites[player->current_frame])/2.2;
        }
        if ((player->current_frame == SHEAVY3) || (player->current_frame == SHEAVY4)) {
            player->hitbox->width = LIGHT_HITBOX;
            player->hitbox->height = LIGHT_HITBOX;
            player->hitbox->x = player->x + ((1 - 2*player->dir) * al_get_bitmap_width(player->sprites[player->current_frame])/1.8) + (1 - player->dir % 2) * (37);
        }
        if ((player->current_frame == DLIGHT1) || (player->current_frame == DLIGHT2)) {
            player->hitbox->width = LIGHT_HITBOX;
            player->hitbox->height = LIGHT_HITBOX;
            player->hitbox->y = player->y - al_get_bitmap_height(player->sprites[player->current_frame])/5;
            player->hitbox->x = player->x + ((1 - 2*player->dir) * al_get_bitmap_width(player->sprites[player->current_frame])/2) + (1 - player->dir % 2) * (15);
        }
        if (player->current_frame == DLIGHT3) {
            player->hit = false;
            player->hitbox->x = player->x;
            player->hitbox->y = player->y;
            player->hitbox->width = START_HITBOX;
            player->hitbox->height = START_HITBOX;
        }
        if ((player->current_frame == DSLIGHT1) || (player->current_frame == DSLIGHT4)) {
            player->hit = false;
            player->hitbox->x = player->x;
            player->hitbox->width = START_HITBOX;
            player->hitbox->height = START_HITBOX;
            player->hitbox->y = player->y - al_get_bitmap_height(player->sprites[player->current_frame])/4;
        }
        if ((player->current_frame == DSLIGHT2) || (player->current_frame == DSLIGHT3)) {
            player->hitbox->width = LIGHT_HITBOX;
            player->hitbox->height = LIGHT_HITBOX;
            player->hitbox->x = player->x + ((1 - 2*player->dir) * al_get_bitmap_width(player->sprites[player->current_frame])/1.9) + (1 - player->dir % 2) * (20);
        }
    }

    if (player->id == ID_SAGAT) {
        if ((player->current_frame == LIGHT1) || (player->current_frame == LIGHT4) || (player->current_frame == SLIGHT0) || (player->current_frame == SLIGHT3)) {
            player->hit = false;
            player->hitbox->x = player->x;
            player->hitbox->y = player->y - al_get_bitmap_height(player->sprites[player->current_frame])/3.7;
            player->hitbox->width = START_HITBOX;
            player->hitbox->height = START_HITBOX;
        }
        if ((player->current_frame == LIGHT2) || (player->current_frame == LIGHT3)) {
            player->hitbox->width = LIGHT_HITBOX;
            player->hitbox->height = LIGHT_HITBOX;
            player->hitbox->x = player->x + ((1 - 2*player->dir) * al_get_bitmap_width(player->sprites[player->current_frame])/1.8) + (1 - player->dir % 2) * 35;
        }
        if ((player->current_frame == SLIGHT1) || (player->current_frame == SLIGHT2)) {
            player->hitbox->width = LIGHT_HITBOX;
            player->hitbox->height = LIGHT_HITBOX;
            player->hitbox->y = player->y - al_get_bitmap_height(player->sprites[player->current_frame])/3.5;
            player->hitbox->x = player->x + ((1 - 2*player->dir) * al_get_bitmap_width(player->sprites[player->current_frame])/1.65) + (1 - player->dir % 2) * 1;
        }
        if ((player->current_frame == HEAVY1) || (player->current_frame == HEAVY4)) {
            player->hit = false;
            player->hitbox->x = player->x;
            player->hitbox->y = player->y + al_get_bitmap_height(player->sprites[player->current_frame])/11;
            player->hitbox->width = START_HITBOX;
            player->hitbox->height = START_HITBOX;
        }
        if ((player->current_frame == HEAVY2) || (player->current_frame == HEAVY3)) {
            player->hitbox->width = LIGHT_HITBOX;
            player->hitbox->height = LIGHT_HITBOX;
            player->hitbox->x = player->x + ((1 - 2*player->dir) * al_get_bitmap_width(player->sprites[player->current_frame])/1.8) + (1 - player->dir % 2) * (27);
        }
        if ((player->current_frame == SHEAVY2) || (player->current_frame == SHEAVY5)) {
            player->hit = false;
            player->hitbox->x = player->x;
            player->hitbox->y = player->y - al_get_bitmap_height(player->sprites[player->current_frame])/3.8;
            player->hitbox->width = START_HITBOX;
            player->hitbox->height = START_HITBOX;
        }
        if ((player->current_frame == SHEAVY3) || (player->current_frame == SHEAVY4)) {
            player->hitbox->width = LIGHT_HITBOX;
            player->hitbox->height = LIGHT_HITBOX;
            player->hitbox->x = player->x + ((1 - 2*player->dir) * al_get_bitmap_width(player->sprites[player->current_frame])/1.8) + (1 - player->dir % 2) * (35);
        }
        if ((player->current_frame == DLIGHT1) || (player->current_frame == DLIGHT2)) {
            player->hitbox->width = LIGHT_HITBOX;
            player->hitbox->height = LIGHT_HITBOX;
            player->hitbox->y = player->y - al_get_bitmap_height(player->sprites[player->current_frame])/4.5;
            player->hitbox->x = player->x + ((1 - 2*player->dir) * al_get_bitmap_width(player->sprites[player->current_frame])/2) + (1 - player->dir % 2) * (15);
        }
        if (player->current_frame == DLIGHT3) {
            player->hit = false;
            player->hitbox->x = player->x;
            player->hitbox->y = player->y;
            player->hitbox->width = START_HITBOX;
            player->hitbox->height = START_HITBOX;
        }
        if ((player->current_frame == DSLIGHT1) || (player->current_frame == DSLIGHT4)) {
            player->hit = false;
            player->hitbox->x = player->x;
            player->hitbox->width = START_HITBOX;
            player->hitbox->height = START_HITBOX;
            player->hitbox->y = player->y - al_get_bitmap_height(player->sprites[player->current_frame])/4;
        }
        if ((player->current_frame == DSLIGHT2) || (player->current_frame == DSLIGHT3)) {
            player->hitbox->width = LIGHT_HITBOX;
            player->hitbox->height = LIGHT_HITBOX;
            player->hitbox->x = player->x + ((1 - 2*player->dir) * al_get_bitmap_width(player->sprites[player->current_frame])/1.9) + (1 - player->dir % 2) * (20);
        }
    }

    if (player->id == ID_CHUNLI) {
        if ((player->current_frame == LIGHT1) || (player->current_frame == LIGHT4) || (player->current_frame == SLIGHT0) || (player->current_frame == SLIGHT3)) {
            player->hit = false;
            player->hitbox->x = player->x;
            player->hitbox->y = player->y - al_get_bitmap_height(player->sprites[player->current_frame])/4.6;
            player->hitbox->width = START_HITBOX;
            player->hitbox->height = START_HITBOX;
        }
        if ((player->current_frame == LIGHT2) || (player->current_frame == LIGHT3)) {
            player->hitbox->width = LIGHT_HITBOX;
            player->hitbox->height = LIGHT_HITBOX;
            player->hitbox->x = player->x + ((1 - 2*player->dir) * al_get_bitmap_width(player->sprites[player->current_frame])/1.8) + (1 - player->dir % 2) * 40;
        }
        if ((player->current_frame == SLIGHT1) || (player->current_frame == SLIGHT2)) {
            player->hitbox->width = LIGHT_HITBOX;
            player->hitbox->height = LIGHT_HITBOX;
            player->hitbox->y = player->y - al_get_bitmap_height(player->sprites[player->current_frame])/3.5;
            player->hitbox->x = player->x + ((1 - 2*player->dir) * al_get_bitmap_width(player->sprites[player->current_frame])/1.8) + (1 - player->dir % 2) * 1;
        }
        if ((player->current_frame == HEAVY1) || (player->current_frame == HEAVY4)) {
            player->hit = false;
            player->hitbox->x = player->x;
            player->hitbox->y = player->y + al_get_bitmap_height(player->sprites[player->current_frame])/(-10);
            player->hitbox->width = START_HITBOX;
            player->hitbox->height = START_HITBOX;
        }
        if ((player->current_frame == HEAVY2) || (player->current_frame == HEAVY3)) {
            player->hitbox->width = LIGHT_HITBOX;
            player->hitbox->height = LIGHT_HITBOX;
            player->hitbox->x = player->x + ((1 - 2*player->dir) * al_get_bitmap_width(player->sprites[player->current_frame])/1.7) + (1 - player->dir % 2) * (15);
        }
        if ((player->current_frame == SHEAVY2) || (player->current_frame == SHEAVY5)) {
            player->hit = false;
            player->hitbox->x = player->x;
            player->hitbox->y = player->y - al_get_bitmap_height(player->sprites[player->current_frame])/3;
            player->hitbox->width = START_HITBOX;
            player->hitbox->height = START_HITBOX;
        }
        if ((player->current_frame == SHEAVY3) || (player->current_frame == SHEAVY4)) {
            player->hitbox->width = LIGHT_HITBOX;
            player->hitbox->height = LIGHT_HITBOX;
            player->hitbox->x = player->x + ((1 - 2*player->dir) * al_get_bitmap_width(player->sprites[player->current_frame])/1.8) + (1 - player->dir % 2) * (35);
        }
        if ((player->current_frame == DLIGHT1) || (player->current_frame == DLIGHT2)) {
            player->hitbox->width = LIGHT_HITBOX;
            player->hitbox->height = LIGHT_HITBOX;
            player->hitbox->y = player->y - al_get_bitmap_height(player->sprites[player->current_frame])/7;
            player->hitbox->x = player->x + ((1 - 2*player->dir) * al_get_bitmap_width(player->sprites[player->current_frame])/2) + (1 - player->dir % 2) * (15);
        }
        if (player->current_frame == DLIGHT3) {
            player->hit = false;
            player->hitbox->x = player->x;
            player->hitbox->y = player->y;
            player->hitbox->width = START_HITBOX;
            player->hitbox->height = START_HITBOX;
        }
        if ((player->current_frame == DSLIGHT1) || (player->current_frame == DSLIGHT4)) {
            player->hit = false;
            player->hitbox->x = player->x;
            player->hitbox->width = START_HITBOX;
            player->hitbox->height = START_HITBOX;
            player->hitbox->y = player->y - al_get_bitmap_height(player->sprites[player->current_frame])/6;
        }
        if ((player->current_frame == DSLIGHT2) || (player->current_frame == DSLIGHT3)) {
            player->hitbox->width = LIGHT_HITBOX;
            player->hitbox->height = LIGHT_HITBOX;
            player->hitbox->x = player->x + ((1 - 2*player->dir) * al_get_bitmap_width(player->sprites[player->current_frame])/1.9) + (1 - player->dir % 2) * (20);
        }
    }
};

/* Reseta os estados e propriedades do personagem. */
void resetChar(character *player) {
    player->frame_counter = 0;
    player->hit = false;
    player->block = false;
    player->hitbox->width = START_HITBOX;
    player->hitbox->height = START_HITBOX;
    player->hitbox->x = player->x;
    player->hitbox->y = player->y;
    player->hurtbox->x = player->x;
    player->hurtbox->y = player->y;
    player->char_render->x = player->x;
    player->char_render->y = player->y;
};

void characterRestart(character *chara, int pl) {
    chara->block = false;
    chara->air_speed = 0;
    chara->current_frame = IDLE0;
    chara->state = IDLE;
    chara->x = (pl == 1)? P1_X : P2_X;
    chara->y = START_Y;
    chara->hitbox->x = chara->x;
    chara->hitbox->y = chara->y;
    chara->hurtbox->x = chara->x;
    chara->hurtbox->y = chara->y;
    chara->char_render->x = chara->x;
    chara->char_render->y = chara->y;
    chara->hitbox->width = START_HITBOX;
    chara->hitbox->height = START_HITBOX;
    chara->hurtbox->width = CHAR_WIDTH;
    chara->hurtbox->height = CHAR_HEIGHT;
    chara->char_render->width = CHAR_WIDTH;
    chara->char_render->height = CHAR_HEIGHT;
    chara->dir = (pl == 1)? RIGHT_DIR : LEFT_DIR;
    chara->control->up = 0;
    chara->control->left = 0;
    chara->control->right = 0;
    chara->control->down = 0;
};

void storageState(character *player) {
    player->previous_state = player->state;
    player->previous_frame = player->current_frame;
    player->previous_delay = player->frame_delay;
};

/* Funções de liberação de memória. */
void deleteSprites(character *chara) {
    if (!(chara->sprites)) return;

    for (int i = 0; i < NUM_SPRITES; ++i) {
        al_destroy_bitmap(chara->sprites[i]);
    }
    free(chara->sprites);
}

void destroyRectangle(rectangle *rect) {
    if (!(rect)) return;
    
    free(rect);
};

void destroyCharacter(character *chara) {
    deleteSprites(chara);
    destroyRectangle(chara->char_render);
    destroyRectangle(chara->hurtbox);
    destroyRectangle(chara->hitbox);
    destroyJoy(chara->control);
    free(chara);
};