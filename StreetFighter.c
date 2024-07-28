#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "render.h"
#include "character.h"

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
    int round = 1;
    bool charge = false;
    bool game = true;
    bool load = false;
    character *player1 = NULL;
    character *player2 = NULL;
    Figure *arrow = createFigure(505, HEIGHT - 210, 0,2, "./figures/attackArrow.bmp");
    Figure *selectionP1 = createFigure(143, HEIGHT - 232, 0, 3, "./figures/selection.bmp");
    Figure *selectionP2 = createFigure(986, HEIGHT - 232, 3, 3, "./figures/selectionRed.bmp");
    Figure *selectionM = createFigure(362, 405, 0, 9, "./figures/selectionMap.bmp");
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
                    selectionScreen(render, selectionP1, selectionP2, selectionM, event, &i, &load);
                    if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                        render->gameMode = START;
                            fade_out(render->display, render->background[i],
                                    0.02);
                            fade_in(render->display, render->background[0], 
                                    0.02);
                        i = 0;
                        selectionP1->itOk = false;
                        selectionP2->itOk = false;
                        selectionM->itOk = false;
                        render->currentBackground = i;
                    }
                    if (load) {
                        player1 = createCharacter(P1_X, START_Y, WIDTH, MAX_Y, selectionP1->op, RIGHT_DIR);
                        player2 = createCharacter(P2_X, START_Y, WIDTH, MAX_Y, selectionP2->op, LEFT_DIR);
                        load = false;
                        charge = true;
                    }
                    break;
                
                case GAME:
                    if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                        render->gameMode = SELECTION;
                            fade_out(render->display, render->background[i],
                                    0.02);
                            fade_in(render->display, render->background[3],
                                    0.02);
                        i = 3;
                        selectionP1->itOk = false;
                        selectionP2->itOk = false;
                        selectionM->itOk = false;
                        selectionM->dx = 362;
                        selectionM->dy = 405;
                        destroyCharacter(player1);
                        destroyCharacter(player2);
                        player1 = NULL;
                        player2 = NULL;
                        selectionM->op = 0;
                        render->currentBackground = i;
                    }
                    charactersMovement(event, player1, player2);
                    break;
                default:
                    break;
            }
        }

        if (event.type == ALLEGRO_EVENT_KEY_UP) {
            if (render->gameMode == GAME) {
                charactersMovement(event, player1, player2);
            }
        }

        if ((redraw) && (al_is_event_queue_empty(render->queue))) {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            switch (render->gameMode){
                case START:
                    drawStart(render, arrow);
                    break;

                case SELECTION:
                    drawSelection(render, selectionP1, selectionP2, selectionM, &i);
                    break;
                
                case GAME:
                    updateAnimation(player1);
                    updateAnimation(player2);
                    update_position(player1, player2);
                    drawGame(render, player1, player2, &i, round, charge);
                    break;
                case ENDGAME:
                    al_draw_filled_rectangle(0, 0, WIDTH, HEIGHT, al_map_rgb(0,0,0));
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
    deleteFigure(selectionM);
    if (player1) destroyCharacter(player1);
    if (player2) destroyCharacter(player2);

    endGame(render);

    return 0;
}