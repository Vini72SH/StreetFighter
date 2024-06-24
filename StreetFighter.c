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

    render = startGame();

    int i = 0;
    bool game = true;
    Figure *arrow = createFigure(505, HEIGHT - 210, 0,2, "./figures/attackArrow.bmp");
    // CONST 281
    Figure *selectionP1 = createFigure(143, HEIGHT - 232, 0, 3, "./figures/selection.bmp");
    Figure *selectionP2 = createFigure(986, HEIGHT - 232, 3, 3, "./figures/selectionRed.bmp");
    bool redraw = true;
    ALLEGRO_EVENT event;

    al_start_timer(render->timer);
    while (game) {
        al_wait_for_event(render->queue, &event);

        if (event.type == ALLEGRO_EVENT_TIMER)
            redraw = true;

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            break;

        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (render->gameMode) {
                case START:
                    startScreen(render, arrow, event, &i);
                    if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                        game = false;
                    break;

                case SELECTION:
                    selectionScreen(render, selectionP1, selectionP2, event, &i);
                    if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                        render->gameMode = START;
                            fade_out(render->display, render->background[i],
                                    0.02);
                            fade_in(render->display, render->background[0], 
                                    0.02);
                        i = 0;
                        selectionP1->itOk = false;
                        selectionP2->itOk = false;
                        render->currentBackground = i;    
                    }

                    if (render->currentBackground == 1) {
                        if (event.keyboard.keycode == ALLEGRO_KEY_Z)
                            selectionP1->itOk = true;
                        if (event.keyboard.keycode == ALLEGRO_KEY_X)
                            selectionP1->itOk = false;
                    } else if (render->currentBackground == 2) {
                        if (event.keyboard.keycode == ALLEGRO_KEY_Z)
                            selectionP1->itOk = true;
                        if (event.keyboard.keycode == ALLEGRO_KEY_X)
                            selectionP1->itOk = false;
                        if (event.keyboard.keycode == ALLEGRO_KEY_PAD_1)
                            selectionP2->itOk = true;
                        if (event.keyboard.keycode == ALLEGRO_KEY_PAD_2)
                            selectionP2->itOk = false;
                    }                    
                    break;
                default:
                    break;
            }
        }

        if ((redraw) && (al_is_event_queue_empty(render->queue))) {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            switch (render->gameMode){
                case START:
                    drawStart(render, arrow);
                    break;

                case SELECTION:
                    drawSelection(render, selectionP1, selectionP2, &i);
                    break;
                default:
                    break;
            }
            redraw = false;
            al_flip_display();
        }
    }

    deleteFigure(arrow);
    deleteFigure(selectionP1);
    deleteFigure(selectionP2);

    endGame(render);

    return 0;
}