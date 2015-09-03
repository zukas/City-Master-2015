#ifndef CONTROLINPUT_H
#define CONTROLINPUT_H

enum navigation : unsigned int {
    NONE = 0,
    ROT_X = 1,
    ROT_Y = 2,
    MOVE_X = 4,
    MOVE_Y = 8,
    ZOOM = 16
};

typedef unsigned int control_value;

constexpr float navigation_delta { 25.f };

struct control {
	control_value val { 0 };
	float delta[5] { 0 };
};

class ControlInput {
  public:
    ControlInput();
	control poll(struct GLFWwindow *window);
};

#endif // CONTROLINPUT_H
