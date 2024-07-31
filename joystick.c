#include <stdlib.h>
#include "joystick.h"

joystick *createJoy() {
    joystick *newControl;

    newControl = malloc(sizeof(joystick));
    if (!(newControl)) return NULL;

    newControl->up = 0;
    newControl->down = 0;
    newControl->left = 0;
    newControl->right = 0;

    return newControl;
};

void joystick_up(joystick *control) {
    control->up = control->up ^ 1;
};

void joystick_down(joystick *control) {
    control->down = control->down ^ 1;
};

void joystick_left(joystick *control) {
    control->left = control->left ^ 1;
};

void joystick_right(joystick *control) {
    control->right = control->right ^ 1;
};

void resetJoy(joystick *control) {
    control->up = 0;
    control->down = 0;
    control->left = 0;
    control->right = 0;
};

void destroyJoy(joystick *control) {
    free(control);
};