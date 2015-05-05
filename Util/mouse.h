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

typedef std::function<void(MoveDirection event, float deltaTime) > MouseDragEvent;
typedef std::function<void(double x, double y) > MouseClickEvent;

class Mouse
{
    private:
        GLFWwindow *m_window { nullptr };
        double m_pressPosX { 0 };
        double m_pressPosY { 0 };
        double m_currPosX { 0 };
        double m_currPosY { 0 };
        bool m_rDrag { false };
        bool m_lDrag { false };
        bool m_lClick { false };
        MouseClickEvent m_eventsClick[3] { nullptr };
        MouseDragEvent m_eventsDrag[2] { nullptr };
    public:
        Mouse(GLFWwindow *window = nullptr);
        ~Mouse();
        void update();
        void onPressLeft(MouseClickEvent e);
        void onReleaseLeft(MouseClickEvent e);
        void onClickLeft(MouseClickEvent e);
        void onDragLeft(MouseDragEvent e);
        void onDragRight(MouseDragEvent e);
};

#endif // MOUSE_H
