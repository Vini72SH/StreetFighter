#ifndef __JOYSTICK__
#define __JOYSTICK__

typedef unsigned char uchar;
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

typedef struct joystick {
    uchar up;
    uchar down;
    uchar left;
    uchar right;
}joystick;

joystick *createJoy();
void joystick_up(joystick *control);
void joystick_down(joystick *control);
void joystick_left(joystick *control);
void joystick_right(joystick *control);
void resetJoy(joystick *control);
void destroyJoy(joystick *control);

#endif