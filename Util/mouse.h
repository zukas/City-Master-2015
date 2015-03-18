#ifndef MOUSE_H
#define MOUSE_H


#include <GLFW/glfw3.h>
#include <functional>

typedef std::function<void(double x, double y) > MouseEvent;

class Mouse
{
    private:
        GLFWwindow *m_window { nullptr };
        double m_posX { 0 };
        double m_posY { 0 };
        MouseEvent m_events[3] { nullptr };

    public:
        Mouse(GLFWwindow *window = nullptr);
        ~Mouse();
        void update();
        void onPress(MouseEvent e);
        void onRelease(MouseEvent e);
        void onclick(MouseEvent e);
};

#endif // MOUSE_H
