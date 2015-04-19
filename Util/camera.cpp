#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "clock.h"


constexpr float min_height { 5.f };
constexpr float max_height { 80.f };

void Camera::calcQuads()
{
    glm::vec3 tmp = m_view-m_eye;
    tmp = glm::normalize(tmp);
    m_quads[0] = glm::cross(tmp, m_up);
    m_quads[0] = glm::normalize(m_quads[0]);
    m_quads[1] = glm::cross(tmp, m_quads[0]);
    m_quads[1] = glm::normalize(m_quads[1]);
}

Camera::Camera(GLFWwindow *window) :
    m_window(window)
{


    m_viewMat = glm::lookAt(
                    m_eye,
                    m_view,
                    m_up);

    m_projectionMat = glm::perspective(45.f, 16.0f / 9.0f, 0.001f, 200.0f);
    calcQuads();
}

void Camera::calcViewport()
{
    if(m_window == nullptr) return;

    m_viewMat = glm::lookAt(m_eye, m_view, m_up);
    if(m_update != 0)
    {
        m_update = 0;
        calcQuads();
    }
}

void Camera::rotateHorizontal(float diff)
{
    m_update = 1;
    m_view -= m_eye;
    m_view = glm::rotate(m_view, 2 * PI * m_sensitivity * diff, glm::vec3(0.f, 1.f, 0.f));
    m_view += m_eye;

}

void Camera::rotateVertical(float delta)
{
    auto diff = m_view-m_eye;
    auto dir = glm::sin( glm::normalize(diff));
    auto tmp = dir.y * delta;
    if(tmp > 0)
        tmp = 0.05;
    else
        tmp = -0.05;
    if(dir.x - tmp >= 0.0 && dir.z - tmp >= 0.0)
        return;

    m_update = 1;
    m_view -= m_eye;
    m_view = glm::rotate(m_view, 2 * PI * m_sensitivity * delta, glm::cross(glm::vec3(diff.x * dir.x, 0.f, diff.z * dir.z), m_up));
    m_view += m_eye;
}

void Camera::move(float delta)
{
    m_update = 1;
    auto diff = m_view - m_eye;
    auto dir = glm::cos(glm::normalize(diff));
    auto tmp = glm::vec3(diff.x * dir.x , 0.f, diff.z * dir.z) * delta * m_velocity;
    m_view += tmp;
    m_eye += tmp;
}
void Camera::strafe(float delta)
{
    m_update = 1;
    auto diff = glm::cross(m_view - m_eye, m_up);
    auto dir = glm::cos(glm::normalize(diff));
    auto tmp = glm::vec3(diff.x * dir.x , 0.f, diff.z * dir.z) * delta * m_velocity;
    m_view += tmp;
    m_eye += tmp;

}

void Camera::zoom(float delta)
{
    auto dir = m_eye * m_up;
    auto tmp = glm::normalize(dir) * delta * m_speed;
    printf("%f, %f\n", m_eye.y, tmp.y);
    fflush(stdout);
    if(m_eye.y - tmp.y < min_height || m_eye.y - tmp.y > max_height)
        return;

    m_update = 1;
//    m_view += tmp;
    m_eye -= tmp;
}

glm::mat4 Camera::view() const
{
    return m_viewMat;
}

glm::mat4 Camera::projection() const
{
    return m_projectionMat;
}

void Camera::update(Program &program)
{
    program.setViewMatrix(m_viewMat);
    program.setProjectionMatrix(m_projectionMat);
    program.setQuads(m_quads);
}
