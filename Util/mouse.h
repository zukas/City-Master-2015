#ifndef MOUSE_H
#define MOUSE_H


#include <GLFW/glfw3.h>
#include <functional>

enum MoveDirection
{
    MOVE_X,
    MOVE_Y,
    MOVE_Z
};

typedef std::function<void(MoveDirection event, float deltaTime) > MouseRightEvent;
typedef std::function<void(double x, double y) > MouseLeftEvent;

class Mouse
{
    private:
        GLFWwindow *m_window { nullptr };
        double m_pressPosX { 0 };
        double m_pressPosY { 0 };
        double m_currPosX { 0 };
        double m_currPosY { 0 };
        bool m_drag { false };
        MouseLeftEvent m_eventsLeft[3] { nullptr };
        MouseRightEvent m_eventsRight[1] { nullptr };
    public:
        Mouse(GLFWwindow *window = nullptr);
        ~Mouse();
        void update();
        void onPressLeft(MouseLeftEvent e);
        void onReleaseLeft(MouseLeftEvent e);
        void onclickLeft(MouseLeftEvent e);
        void onDragRight(MouseRightEvent e);
};

#endif // MOUSE_H
