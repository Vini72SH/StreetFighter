#include "render.h"

void init(bool test, const char *description) {
    if(test) return;

    printf("Não foi possível inicializar %s\n", description);
    exit(1);
};

void fade_out(ALLEGRO_DISPLAY *display, ALLEGRO_BITMAP *current_bitmap, float speed) {
    for (float alpha = 1.0; alpha >= 0.0; alpha -= speed) {
        al_draw_scaled_bitmap(current_bitmap, 0, 0,
                              al_get_bitmap_width(current_bitmap),
                              al_get_bitmap_height(current_bitmap),
                              0, 0, WIDTH, HEIGHT, 0);
        al_draw_filled_rectangle(0, 0, WIDTH, HEIGHT, al_map_rgba_f(0, 0, 0, 1 - alpha));
        al_flip_display();
        al_rest(0.01); 
    }
};

void fade_in(ALLEGRO_DISPLAY *display, ALLEGRO_BITMAP *next_bitmap, float speed) {
    for (float alpha = 0.0; alpha <= 1.0; alpha += speed) {
        al_draw_scaled_bitmap(next_bitmap, 0, 0, 
                              al_get_bitmap_width(next_bitmap),
                              al_get_bitmap_height(next_bitmap),
                              0, 0, WIDTH, HEIGHT, 0);
        al_draw_filled_rectangle(0, 0, WIDTH, HEIGHT, al_map_rgba_f(0, 0, 0, 1 - alpha));
        al_flip_display();
        al_rest(0.01); 
    }
};

Screen_Render *startGame() {
    Screen_Render *newScreen;
    const char *filenames[IMAGES] = {
        "./images/intro.png",
        "./images/ScreenCOM.bmp",
        "./images/ScreenCOOP.bmp"
    };

    const char *names[CHARACTERS] = {
        "./characters/RyuFace.bmp",
        "./characters/KenFace.bmp",
        "./characters/SagatFace.bmp",
        "./characters/ChunLiFace.bmp"
    };
    
    init(al_install_keyboard(), "keyboard");
    init(al_init_image_addon(), "image addon");
    init(al_init_primitives_addon(), "primitives");
    init(al_init_font_addon(), "font addon");
    init(al_init_ttf_addon(), "ttf addon");
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    newScreen = malloc(sizeof(Screen_Render));
    init(newScreen, "ponteiro para tela");

    newScreen->timer = al_create_timer(1.0 / FRAMERATE);
    init(newScreen->timer, "timer");

    newScreen->queue = al_create_event_queue();
    init(newScreen->queue, "queue");

    newScreen->font1 = al_load_ttf_font("./fonts/game.ttf", 32, 0);
    init(newScreen->font1, "font1");
    newScreen->font2 = al_load_ttf_font("./fonts/alerts.ttf", 32, 0);
    init(newScreen->font2, "font2");

    newScreen->display = al_create_display(WIDTH, HEIGHT);
    init(newScreen->display, "display");

    al_set_window_title(newScreen->display, "Street Fighter");
    al_set_window_position(newScreen->display, 20, 0);
    al_hide_mouse_cursor(newScreen->display);

    al_register_event_source(newScreen->queue, al_get_keyboard_event_source());
    al_register_event_source(newScreen->queue, al_get_display_event_source(newScreen->display));
    al_register_event_source(newScreen->queue, al_get_timer_event_source(newScreen->timer));

    newScreen->gameMode = START;
    newScreen->currentBackground = 0;

    for (int i = 0; i < IMAGES; ++i) {
        newScreen->background[i] = al_load_bitmap(filenames[i]);
        init(newScreen->background[i], filenames[i]);
    }

    for (int i = 0; i < CHARACTERS; ++i) {
        newScreen->chars[i] = al_load_bitmap(names[i]);
        init(newScreen->chars[i], names[i]);
    }

    return newScreen;
};

Figure *createFigure(int dx, int dy, int op, int max_op, const char *filename) {
    Figure *newFigure;

    newFigure = malloc(sizeof(Figure));
    init(newFigure, filename);

    newFigure->image = al_load_bitmap(filename);
    init(newFigure->image, filename);

    newFigure->dx = dx;
    newFigure->dy = dy;
    newFigure->w = al_get_bitmap_width(newFigure->image);
    newFigure->h = al_get_bitmap_height(newFigure->image);
    newFigure->op = op;
    newFigure->max_op = max_op;
    newFigure->itOk = false;
    newFigure->move = 0;

    return newFigure;
};

void drawStart(Screen_Render *render, Figure *arrow) {
    ALLEGRO_COLOR red = al_map_rgb(255, 25, 35);
    ALLEGRO_COLOR white = al_map_rgb(255, 255, 255);

    al_draw_scaled_bitmap(render->background[0],
                            0, 0, al_get_bitmap_width(render->background[0]),
                            al_get_bitmap_height(render->background[0]),
                            0, 0, WIDTH, HEIGHT, 0);

    al_draw_text(render->font2, white, 0, HEIGHT - 100, 0, "PRESS Z TO SELECT");
    al_draw_text(render->font2, white, 0, HEIGHT - 50, 0, "PRESS ESC TO EXIT");
    if (arrow->op == 0) 
        al_draw_text(render->font1, white, WIDTH/2 -13 * 6, HEIGHT - 200, 0, "SINGLEPLAYER");
    else
        al_draw_text(render->font1, red, WIDTH/2 -13 * 6, HEIGHT - 200, 0, "SINGLEPLAYER");

    if (arrow->op == 1) 
        al_draw_text(render->font1, white, WIDTH/2 -12 * 6, HEIGHT - 150, 0, "MULTIPLAYER");
    else
        al_draw_text(render->font1, red, WIDTH/2 -12 * 6, HEIGHT - 150, 0, "MULTIPLAYER");

    if (arrow->op == 2) 
        al_draw_text(render->font1, white, WIDTH/2 -8 * 6, HEIGHT - 100, 0, "OPTIONS");
    else
        al_draw_text(render->font1, red, WIDTH/2 -8 * 6, HEIGHT - 100, 0, "OPTIONS");

    al_draw_scaled_bitmap(arrow->image, 0, 0,
                            arrow->w, arrow->h, 
                            arrow->dx, arrow->dy, 
                            48, 48, 0);
    if (arrow->move == 15) arrow->move = -14;
    if (arrow->move < 0) arrow->dx--;
    if (arrow->move > 0) arrow->dx++;
    arrow->move++;
};

void startScreen(Screen_Render *render, Figure *arrow, ALLEGRO_EVENT event, int *i) {
    if (event.keyboard.keycode == ALLEGRO_KEY_W) 
        if (arrow->op > 0) {
                arrow->op--;
                arrow->dy -= 50;
            }

    if (event.keyboard.keycode == ALLEGRO_KEY_S)
        if (arrow->op < arrow->max_op) {
                arrow->op++;
                arrow->dy += 50;
            }

    if (event.keyboard.keycode == ALLEGRO_KEY_Z) {
        render->gameMode = SELECTION;
        if (arrow->op == 0) {
            fade_out(render->display, render->background[0],
                    0.02);
            fade_in(render->display, render->background[1], 
                    0.02);    
            *i = 1;
        } else if (arrow->op == 1) {
            fade_out(render->display, render->background[0],
                    0.02);
            fade_in(render->display, render->background[2], 
                    0.02);
            *i = 2;
        }
        render->currentBackground = *i;
    }
};

void drawSelection(Screen_Render *render, Figure *s1, Figure *s2, int *i) {
    ALLEGRO_COLOR white = al_map_rgb(255, 255, 255);
    if (render->currentBackground == 1) {
        al_draw_scaled_bitmap(render->background[1],
                                0, 0, al_get_bitmap_width(render->background[1]),
                                al_get_bitmap_height(render->background[1]),
                                0, 0, WIDTH, HEIGHT, 0);
        al_draw_scaled_bitmap(s1->image, 0, 0, s1->w, s1->h, s1->dx, s1->dy, 200, 200, 0);
        al_draw_scaled_bitmap(render->chars[s1->op], 0, 0,
                        al_get_bitmap_width(render->chars[s1->op]), 
                        al_get_bitmap_height(render->chars[s1->op]),
                        WIDTH - 1250, HEIGHT - 541, 380, 250, 0);

        if (s1->op == 0) 
            al_draw_text(render->font2, white, WIDTH - 1250, HEIGHT - 550, 0, "Ryu");
        else if (s1->op == 1)
            al_draw_text(render->font2, white, WIDTH - 1250, HEIGHT - 550, 0, "Ken");
        else if (s1->op == 2)
            al_draw_text(render->font2, white, WIDTH - 1250, HEIGHT - 550, 0, "Sagat");
        else if (s1->op == 3)
            al_draw_text(render->font2, white, WIDTH - 1250, HEIGHT - 550, 0, "Chun Li");

    } else if (render->currentBackground == 2) {
        al_draw_scaled_bitmap(render->background[2],
                                0, 0, al_get_bitmap_width(render->background[2]),
                                al_get_bitmap_height(render->background[2]),
                                0, 0, WIDTH, HEIGHT, 0);
        al_draw_scaled_bitmap(s2->image, 0, 0, s2->w, s2->h, s2->dx, s2->dy, 200, 200, 0);
        al_draw_scaled_bitmap(s1->image, 0, 0, s1->w, s1->h, s1->dx, s1->dy, 200, 200, 0);

        al_draw_scaled_bitmap(render->chars[s1->op], 0, 0,
                al_get_bitmap_width(render->chars[s1->op]), 
                al_get_bitmap_height(render->chars[s1->op]),
                WIDTH - 1250, HEIGHT - 541, 380, 250, 0);

        al_draw_scaled_bitmap(render->chars[s2->op], 0, 0,
                al_get_bitmap_width(render->chars[s2->op]), 
                al_get_bitmap_height(render->chars[s2->op]),
                WIDTH - 460, HEIGHT - 541, 380, 250, ALLEGRO_ALIGN_CENTER);

        if (s1->op == 0) 
            al_draw_text(render->font2, white, WIDTH - 1250, HEIGHT - 550, 0, "Ryu");
        else if (s1->op == 1)
            al_draw_text(render->font2, white, WIDTH - 1250, HEIGHT - 550, 0, "Ken");
        else if (s1->op == 2)
            al_draw_text(render->font2, white, WIDTH - 1250, HEIGHT - 550, 0, "Sagat");
        else if (s1->op == 3)
            al_draw_text(render->font2, white, WIDTH - 1250, HEIGHT - 550, 0, "Chun Li");

        if (s2->op == 0) 
            al_draw_text(render->font2, white, WIDTH - 130, HEIGHT - 550, 0, "Ryu");
        else if (s2->op == 1)
            al_draw_text(render->font2, white, WIDTH - 130, HEIGHT - 550, 0, "Ken");
        else if (s2->op == 2)
            al_draw_text(render->font2, white, WIDTH - 130, HEIGHT - 550, 0, "Sagat");
        else if (s2->op == 3)
            al_draw_text(render->font2, white, WIDTH - 130, HEIGHT - 550, 0, "Chun Li");
    }
};

void selectionScreen(Screen_Render *render, Figure *s1, Figure *s2, ALLEGRO_EVENT event, int *i) {
    if (render->currentBackground == 1) {
        if (event.keyboard.keycode == ALLEGRO_KEY_D) {
            if ((s1->op < s1->max_op) && (!(s1->itOk))) { 
                s1->dx += 281;
                s1->op++;
            }
        }
        if (event.keyboard.keycode == ALLEGRO_KEY_A) {
            if ((s1->op > 0) && (!(s1->itOk))) {
                s1->dx -= 281;
                s1->op--;
            }
        }
    } else if (render->currentBackground == 2) {
        if (event.keyboard.keycode == ALLEGRO_KEY_D) {
            if ((s1->op < s1->max_op) && (!(s1->itOk))) { 
                s1->dx += 281;
                s1->op++;
            }
        }
        if (event.keyboard.keycode == ALLEGRO_KEY_A) {
            if ((s1->op > 0) && (!(s1->itOk))) {
                s1->dx -= 281;
                s1->op--;
            }
        }
        if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
            if ((s2->op < s2->max_op) && (!(s2->itOk))) { 
                s2->dx += 281;
                s2->op++;
            }
        }
        if (event.keyboard.keycode == ALLEGRO_KEY_LEFT) {
            if ((s2->op > 0) && (!(s2->itOk))) {
                s2->dx -= 281;
                s2->op--;
            }
        }
    }
};

void deleteFigure(Figure *figure) {
    al_destroy_bitmap(figure->image);
    
    free(figure);
};

void endGame(Screen_Render *screen) {
    if(!(screen)) return;

    for (int i = 0; i < IMAGES; ++i) {
        al_destroy_bitmap(screen->background[i]);
    }

    for (int i = 0; i < CHARACTERS; ++i) {
        al_destroy_bitmap(screen->chars[i]);
    }

    al_destroy_display(screen->display);
    al_destroy_font(screen->font1);
    al_destroy_event_queue(screen->queue);
    al_destroy_timer(screen->timer);

    free(screen);

};