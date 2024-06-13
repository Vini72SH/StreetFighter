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

    gameRender(render);

    endGame(render);

    return 0;
}