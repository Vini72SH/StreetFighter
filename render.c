#include "render.h"


void init(bool test, const char *description) {
    if(test) return;

    printf("Não foi possível inicializar %s\n", description);
    exit(1);
};

Screen_Render *startGame() {
    Screen_Render *newScreen;
    
    init(al_install_keyboard(), "keyboard");
    init(al_init_image_addon(), "image addon");
    init(al_init_primitives_addon(), "primitives");
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    newScreen = malloc(sizeof(Screen_Render));
    init(newScreen, "ponteiro para tela");

    newScreen->timer = al_create_timer(1.0 / FRAMERATE);
    init(newScreen->timer, "timer");

    newScreen->queue = al_create_event_queue();
    init(newScreen->queue, "queue");

    newScreen->font = al_create_builtin_font();
    init(newScreen->font, "font");

    newScreen->display = al_create_display(WIDTH, HEIGHT);
    init(newScreen->display, "display");

    al_set_window_title(newScreen->display, "Street Fighter");
    al_set_window_position(newScreen->display, 20, 0);
    al_hide_mouse_cursor(newScreen->display);

    al_register_event_source(newScreen->queue, al_get_keyboard_event_source());
    al_register_event_source(newScreen->queue, al_get_display_event_source(newScreen->display));
    al_register_event_source(newScreen->queue, al_get_timer_event_source(newScreen->timer));

    newScreen->currentBackground = 0;
    const char *filenames[IMAGES] = {
        "./images/logo.bmp"
    };

    for (int i = 0; i < IMAGES; ++i) {
        newScreen->background[i] = al_load_bitmap(filenames[i]);
        init(newScreen->background[i], filenames[i]);
    }

    return newScreen;

};

void gameRender(Screen_Render *screen) {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    switch (screen->currentBackground){
        case 0:
        al_draw_scaled_bitmap(screen->background[0], 0, 0,
                              al_get_bitmap_width(screen->background[0]),
                              al_get_bitmap_height(screen->background[0]),
                              450, 100, 450, 350, 0);
        al_draw_text(screen->font, al_map_rgb(255, 255, 255), 660, 460, 0, "START");
        al_draw_text(screen->font, al_map_rgb(255, 255, 255), 635, 480, 0, "MULTIPLAYER");
        al_draw_text(screen->font, al_map_rgb(255, 255, 255), 650, 500, 0, "OPTIONS");
        break;
    
    default:
        break;
    }
    al_flip_display();
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