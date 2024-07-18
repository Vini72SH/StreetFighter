#include "character.h"

void resetCounter(int *counter) {
    (*counter) = 0;
};

ALLEGRO_BITMAP **loadSprites(short id){
    ALLEGRO_BITMAP **sprites;

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

character *createCharacter(ushort x, ushort y, ushort max_x, ushort max_y, short op, short dir) {
    character *newCharacter;
    ALLEGRO_COLOR color;
    ALLEGRO_COLOR blue = al_map_rgb(0, 0, 255);
    ALLEGRO_COLOR red = al_map_rgb(255, 0, 0);
    ALLEGRO_COLOR black = al_map_rgb(0, 0, 0);
    ALLEGRO_COLOR white = al_map_rgb(255, 255, 255);

    if (op == 0) color = blue;
    if (op == 1) color = red;
    if (op == 2) color = black;
    if (op == 3) color = white;
    
    if ((x - CHAR_WIDTH/2 < 0) || (x + CHAR_WIDTH/2 > max_x) || (y - CHAR_HEIGHT/2 < 0) || (y + CHAR_HEIGHT/2 > max_y)) return NULL;

    newCharacter = malloc(sizeof(character));
    if (!(newCharacter)) return NULL;

    newCharacter->hp = HP;
    newCharacter->x = x;
    newCharacter->y = y;
    newCharacter->air_speed = 0;
    newCharacter->dir = dir;
    newCharacter->state = IDLE;
    newCharacter->current_frame = IDLE0;
    newCharacter->frame_delay = FRAME_DELAY;
    newCharacter->frame_counter = 0;
    newCharacter->hurtbox = createRectangle(x, y, CHAR_WIDTH, CHAR_HEIGHT);
    newCharacter->control = createJoy();
    newCharacter->color = color;
    newCharacter->sprites = loadSprites(op);

    return newCharacter;
};

void characterUp(character *chara) {
    if (!(chara)) return;

    if (chara->hurtbox->height != CHAR_HEIGHT) {
        chara->y = chara->y - (CHAR_HEIGHT/2 - CHAR_DOWN_HEIGHT/2);
        chara->hurtbox->y = chara->y;
    }

    if (chara->state == DOWN) {
        chara->state = IDLE;
        chara->current_frame = IDLE0;
    }
    chara->hurtbox->width = CHAR_WIDTH;
    chara->hurtbox->height = CHAR_HEIGHT;
};

void characterDown(character *chara) {
    if (!(chara)) return;

    if (chara->hurtbox->height != CHAR_DOWN_HEIGHT) {
        chara->y = chara->y + (CHAR_HEIGHT/2 - CHAR_DOWN_HEIGHT/2);
        chara->hurtbox->y = chara->y;
    }
    
    chara->hurtbox->width = CHAR_DOWN_WIDTH;
    chara->hurtbox->height = CHAR_DOWN_HEIGHT;
};

void characterMove(character *chara, float steps, ushort trajectory, ushort max_x, ushort max_y) {
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
        chara->y = chara->y - steps * chara->air_speed * JUMP_CONST;
        chara->hurtbox->y = chara->y;
  
    }

    if (trajectory == DOWN) {
        if ((chara->y + steps*SPEED) + chara->hurtbox->height/2 <= max_y) {
            chara->y = chara->y + steps * SPEED;
            chara->hurtbox->y = chara->y;
        }
    }
};

void characterFlush(character *p1, character *p2, ushort max_x, ushort max_y) {
    if (p1->x <= p2->x) {
        characterMove(p1, 1.5, LEFT, max_x, max_y);
    } else {
        characterMove(p1, 1.5, RIGHT, max_x, max_y);
    }
};

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
    }
};

void invertDirections(character *p1, character *p2) {
    if (p1->x > p2->x) {
        p1->dir = LEFT_DIR;
    }else{
        p1->dir = RIGHT_DIR;
    }
};

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
    destroyRectangle(chara->hurtbox);
    destroyJoy(chara->control);
    free(chara);
};