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
        glm::mat4 m_viewMat;
        glm::mat4 m_projectionMat;
//        glm::mat4 m_rotationMat;
        glm::vec3 m_eye { 12.f, 16.f, 14.f };
        glm::vec3 m_view { 1.f, 1.f, 1.f };
        glm::vec3 m_up { 0.f, 1.f, 0.f };
        float m_speed { 25.f };
        float m_sensitivity { 0.1f };
        float m_velocity { m_speed * m_sensitivity };

//        glm::vec3 m_position { startPosition[0], startPosition[1], startPosition[2] };
//        glm::vec3 m_direction { -startPosition[0], -startPosition[1], -startPosition[2] };
        glm::vec3 m_quads[2];
        int m_update { 0 };

//        float m_horizontalAngle { PI / 3.f };
//        float m_verticalAngle { PI / 16.f };
//        float m_mouseSpeed { 0.002f };
//        float m_speed { 3.f };

    private:
        void calcQuads();

    public:
        Camera(GLFWwindow *window = nullptr);
        void calcViewport();
        void rotateHorizontal(float diff);
        void rotateVertical(float diff);
        void move(float diff);
        void strafe(float diff);
        void zoom(float diff);
        glm::mat4 view() const;
        glm::mat4 projection() const;
//        glm::vec3 position() const;
//        glm::vec3 direction() const;
        void update(Program &program);


};

#endif // CAMERA_H
