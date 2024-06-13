#include "render.h"

void init(bool test, const char *description) {
    if(test) return;

    printf("Não foi possível inicializar %s\n", description);
    exit(1);
};

Screen_Render *startGame() {
    Screen_Render *newScreen;
    const char *filenames[IMAGES] = {
        "./images/logo.bmp"
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

    newScreen->font = al_load_ttf_font("./fonts/main.ttf", 32, 0);
    init(newScreen->font, "font");

    newScreen->display = al_create_display(WIDTH, HEIGHT);
    init(newScreen->display, "display");

    al_set_window_title(newScreen->display, "Street Fighter");
    al_set_window_position(newScreen->display, 20, 0);
    al_hide_mouse_cursor(newScreen->display);

    al_register_event_source(newScreen->queue, al_get_keyboard_event_source());
    al_register_event_source(newScreen->queue, al_get_display_event_source(newScreen->display));
    al_register_event_source(newScreen->queue, al_get_timer_event_source(newScreen->timer));

    newScreen->gameMode = MENU;
    newScreen->currentBackground = 0;

    for (int i = 0; i < IMAGES; ++i) {
        newScreen->background[i] = al_load_bitmap(filenames[i]);
        init(newScreen->background[i], filenames[i]);
    }

    return newScreen;

};

Figure *createFigure(int dx, int dy, int max_op, const char *filename) {
    Figure *newFigure;

    newFigure = malloc(sizeof(Figure));
    init(newFigure, filename);

    newFigure->image = al_load_bitmap(filename);
    init(newFigure->image, filename);

    newFigure->dx = dx;
    newFigure->dy = dy;
    newFigure->w = al_get_bitmap_width(newFigure->image);
    newFigure->h = al_get_bitmap_height(newFigure->image);
    newFigure->op = 0;
    newFigure->max_op = max_op;
    newFigure->move = 0;

    return newFigure;
};

void renderMainMenu(Screen_Render *render) {
    al_draw_scaled_bitmap(render->background[0], 0, 0,
                    al_get_bitmap_width(render->background[0]),
                    al_get_bitmap_height(render->background[0]),
                    450, 100, 450, 350, 0);
    al_draw_text(render->font, al_map_rgb(255, 43, 0), 630, 460, 0, "START");
    al_draw_text(render->font, al_map_rgb(255, 43, 0), 580, 510, 0, "MULTIPLAYER");
    al_draw_text(render->font, al_map_rgb(255, 43, 0), 615, 560, 0, "OPTIONS");
};

void gameRender(Screen_Render *render) {
    Figure *arrow = createFigure(505, 450, 2, "./figures/arrow.bmp");
    bool redraw = true;
    ALLEGRO_EVENT event;

    al_start_timer(render->timer);
    while (1) {
        al_wait_for_event(render->queue, &event);

        if (event.type == ALLEGRO_EVENT_TIMER)
            redraw = true;

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            break;

        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (render->gameMode) {
                case MENU:
                    if (event.keyboard.keycode == ALLEGRO_KEY_UP) 
                        if (arrow->op > 0) {
                                arrow->op--;
                                arrow->dy -= 50;
                            }

                    if (event.keyboard.keycode == ALLEGRO_KEY_DOWN)
                        if (arrow->op < arrow->max_op) {
                                arrow->op++;
                                arrow->dy += 50;
                            }
                    break;
            
                default:
                    break;
            }
        }

        if ((redraw) && (al_is_event_queue_empty(render->queue))) {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            switch (render->currentBackground){
                case 0:
                    renderMainMenu(render);
                    al_draw_scaled_bitmap(arrow->image, 0, 0,
                                          arrow->w, arrow->h, 
                                          arrow->dx, arrow->dy, 
                                          48, 48, 0);
                    if (arrow->move == 15) arrow->move = -15;
                    if (arrow->move < 0) arrow->dx--;
                    if (arrow->move > 0) arrow->dx++;
                    arrow->move++;

                    break;
                default:
                    break;
            }
            redraw = false;
            al_flip_display();
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

    al_destroy_display(screen->display);
    al_destroy_font(screen->font);
    al_destroy_event_queue(screen->queue);
    al_destroy_timer(screen->timer);

    free(screen);

};