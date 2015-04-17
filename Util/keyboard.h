#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <GLFW/glfw3.h>
#include <functional>

typedef std::function<void(float deltaTime) > KeyboardEvent;

class Keyboard
{
    private:
        GLFWwindow *m_window { nullptr };
        double m_posX { 0 };
        double m_posY { 0 };
        KeyboardEvent m_events[4] { nullptr };

    public:
        Keyboard(GLFWwindow *window = nullptr);
        ~Keyboard();
        void update();
        void rotate(KeyboardEvent e);
        void moveX(KeyboardEvent e);
        void moveY(KeyboardEvent e);
        void moveZ(KeyboardEvent e);
};

#endif // KEYBOARD_H