#include <stdio.h>
#include <stdlib.h>
#include "render.h"

void must_init(bool test, const char *description) {
    if(test) return;

    printf("Não foi possível inicializar %s\n", description);
    exit(1);
}

int main () {
    Screen_Render *render;
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");

    render = startGame();

    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT event;
    al_start_timer(render->timer);
    while (1) {
        al_wait_for_event(render->queue, &event);

        switch (event.type) {
            case ALLEGRO_EVENT_TIMER:
                redraw = true;
                break;

                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true; 
                break;
        }

        if (done)
            break;

        if ((redraw) && (al_is_event_queue_empty(render->queue))) {
            gameRender(render);

            redraw = false;
        }									
    }

    endGame(render);

    return 0;
}