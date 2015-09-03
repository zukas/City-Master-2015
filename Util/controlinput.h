#ifndef CONTROLINPUT_H
#define CONTROLINPUT_H

enum navigation : unsigned int {
    NONE = 0,
    ROT_X_P = 1,
    ROT_X_N = 2,
    ROT_Y_P = 4,
    ROT_Y_N = 8,
    MOVE_X_P = 16,
    MOVE_X_N = 32,
    MOVE_Y_P = 64,
    MOVE_Y_N = 128,
    ZOOM_P = 256,
    ZOOM_N = 512
};

typedef unsigned int control_value;

struct control {
    control_value val;
    float delta;
};

class ControlInput {
  public:
    ControlInput();
    control poll(struct GLFWwindow *window);
};

#endif // CONTROLINPUT_H
