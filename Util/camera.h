#ifndef CAMERA_H
#define CAMERA_H

#include "types.h"
#include "program.h"
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Camera
{
		static constexpr float max_distance { 75.f };
		static constexpr float min_height { 5.f };

    private:
        GLFWwindow *m_window;
        glm::mat4 m_viewMat;
        glm::mat4 m_projectionMat;
        glm::vec3 m_eye { 12.f, 16.f, 14.f };
        glm::vec3 m_view { 1.f, 1.f, 1.f };
        glm::vec3 m_up { 0.f, 1.f, 0.f };
		glm::vec3 m_quads[2];
        float m_speed { 25.f };
		float m_sensitivity { 0.1f };
        float m_velocity { m_speed * m_sensitivity };
        float m_fov { 45.f };
        int m_update { 0 };

    private:
        void calcQuads();

    public:
        Camera(GLFWwindow *window = nullptr);
        void calcViewport();
		void rotateHorizontal(float delta);
		void rotateVertical(float delta);
		void move(float delta);
		void strafe(float delta);
		void zoom(float delta);
        glm::mat4 view() const;
        glm::mat4 projection() const;
		float distance() const;
        void update(Program &program);


};

#endif // CAMERA_H
