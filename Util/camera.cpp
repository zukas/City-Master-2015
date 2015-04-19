#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "clock.h"
#include <iostream>

Camera::Camera(GLFWwindow *window) :
    m_window(window)
{
    m_view = glm::lookAt(
    m_position,
    glm::vec3 { 0, 0, 0 },
    glm::vec3 { 0, 1, 0 });
    m_projection = glm::perspective(45.f, 16.0f / 9.0f, 0.001f, 200.0f);
    m_direction = {-startPosition[0], -startPosition[1], -startPosition[2] };

    m_direction *= glm::vec3(
                     cos(m_horizontalAngle),
                     sin(m_verticalAngle),
                     sin(m_horizontalAngle)
                     );
    m_up = glm::vec3(0.f, m_speed, 0.f) * glm::vec3(0, sin(m_verticalAngle), 0);

}

void Camera::calcViewport()
{
    if(m_window == nullptr) return;

    if(m_position.y < 5.f)
    {
        m_position.y = 5.f;
    } else if(m_position.y > 80.f)
    {
        m_position.y = 80.f;
    }

    m_view = glm::lookAt(
                 m_position,           // Camera is here
                 m_position+m_direction, // and looks here : at the same position, plus "direction"
                 std::move(m_up)                  // Head is up (set to 0,-1,0 to look upside-down)
                 );

}

void Camera::rotateHorizontal(float diff)
{
    m_horizontalAngle -= diff;
    m_direction = {-startPosition[0], -startPosition[1], -startPosition[2] };

    m_direction *= glm::vec3(
                     cos(m_horizontalAngle),
                     sin(m_verticalAngle),
                     sin(m_horizontalAngle)
                     );
}

void Camera::rotateVertical(float diff)
{
    m_verticalAngle += diff;

    m_direction = {-startPosition[0], -startPosition[1], -startPosition[2] };

    m_direction *= glm::vec3(
                     cos(m_horizontalAngle),
                     sin(m_verticalAngle),
                     sin(m_horizontalAngle)
                     );
}

void Camera::moveX(float diff)
{
    m_position += glm::vec3(m_direction.x, 0.f, m_direction.z) * diff * m_speed;
}
void Camera::moveY(float diff)
{
    static glm::vec3 right = glm::vec3(
                          sin(m_horizontalAngle + PI / 2.f) * PI * m_speed,
                          0,
                          cos(m_horizontalAngle + PI / 2.f) * PI * m_speed
                          );
    m_position += right * diff * m_speed;

}

void Camera::moveZ(float diff)
{
    m_up = glm::vec3(0.f, m_speed, 0.f) * glm::vec3(0, sin(m_verticalAngle), 0);
    m_position += m_up * diff * m_speed * PI;
}

glm::mat4 Camera::view() const
{
    return m_view;
}

glm::mat4 Camera::projection() const
{
    return m_projection;
}

glm::vec3 Camera::position() const
{
    return m_position;
}

glm::vec3 Camera::direction() const
{
    return m_direction;
}

void Camera::update(Program &program)
{
    program.setViewMatrix(m_view);
    program.setProjectionMatrix(m_projection);
//    program.setUniform(glsl_view_matrix, m_view);
//    program.setUniform(glsl_projection_matrix, m_projection);
}
