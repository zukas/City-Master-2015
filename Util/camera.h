#ifndef CAMERA_H
#define CAMERA_H

#include "types.h"
#include "program.h"
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

constexpr float startPosition[3] { 12.f, 16.f, 14.f };

class Camera
{
    private:
        GLFWwindow *m_window;
        glm::mat4 m_view;
        glm::mat4 m_projection;
        glm::mat4 m_rotation;

        glm::vec3 m_position { startPosition[0], startPosition[1], startPosition[2] };
        glm::vec3 m_direction { -startPosition[0], -startPosition[1], -startPosition[2] };
        float m_horizontalAngle { PI / 3.f };
        float m_verticalAngle { PI / 1.f };
        float m_mouseSpeed { 0.002f };
        float m_speed { 3.f };

    public:
        Camera(GLFWwindow *window = nullptr);
        void calcViewport();
        glm::mat4 view() const;
        glm::mat4 projection() const;
        glm::vec3 position() const;
        glm::vec3 direction() const;
        void update(Program &program);

};

#endif // CAMERA_H
