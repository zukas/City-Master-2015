#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "clock.h"

void Camera::calcQuads()
{
//    glm::vec3 tmp = m_view-m_eye;
//    tmp = glm::normalize(tmp);
//    m_quads[0] = glm::cross(tmp, m_up);
//    m_quads[0] = glm::normalize(m_quads[0]);
//    m_quads[1] = glm::cross(tmp, m_quads[0]);
//    m_quads[1] = glm::normalize(m_quads[1]);
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

    //    static double lastTime = glfwGetTime();
    //    double currentTime = glfwGetTime();
    //    float deltaTime = float(currentTime - lastTime);
    //    lastTime = currentTime;


    //    // Get view direction
    //    glm::vec3 vMove = m_view-m_eye;
    //    vMove = glm::normalize(vMove);
    //    vMove *= m_speed;

    //    glm::vec3 vStrafe = glm::cross(m_view-m_eye, m_up);
    //    vStrafe = glm::normalize(vStrafe);
    //    vStrafe *= m_speed;

    //    glm::vec3 vMoveBy;
    //    // Get vector of move


    //    if (glfwGetKey( m_window, GLFW_KEY_UP ) == GLFW_PRESS)
    //    {
    //        vMoveBy += vMove*deltaTime;
    //    }
    //    else if (glfwGetKey( m_window, GLFW_KEY_DOWN ) == GLFW_PRESS)
    //    {
    //        vMoveBy -= vMove*deltaTime;
    //    }

    //    if (glfwGetKey( m_window, GLFW_KEY_RIGHT ) == GLFW_PRESS)
    //    {
    //        vMoveBy += vStrafe*deltaTime;
    //    }
    //    else if (glfwGetKey( m_window, GLFW_KEY_LEFT ) == GLFW_PRESS)
    //    {
    //        vMoveBy -= vStrafe*deltaTime;
    //    }


    //    m_eye += vMoveBy;
    //    m_view += vMoveBy;


//    if (glfwGetKey( m_window, GLFW_KEY_A ) == GLFW_PRESS)
//    {
//        m_view -= m_eye;
//        m_view = glm::rotate(m_view, 2 * PI * m_sensitivity * deltaTime, glm::vec3(0.f, 1.f, 0.f));
//        m_view += m_eye;
//    }
//    else if (glfwGetKey( m_window, GLFW_KEY_D ) == GLFW_PRESS)
//    {
//        m_view -= m_eye;
//        m_view = glm::rotate(m_view, -2 * PI * m_sensitivity * deltaTime, glm::vec3(0.f, 1.f, 0.f));
//        m_view += m_eye;
//    }

    m_viewMat = glm::lookAt(m_eye, m_view, m_up);
    if(m_update != 0)
    {
        m_update = 0;
        calcQuads();
    }


//    m_position.
//    glm::distance(glm::vec3(0.f,0.f,0.f), m_position);
//    printf("Position: (%f, %f, %f)\n", m_position.x, m_position.y, m_position.z);

    //    direction *= glm::vec3(
    //                     cos(m_horizontalAngle),
    //                     sin(m_verticalAngle),
    //                     sin(m_horizontalAngle)
    //                     );

    //    glm::vec3 right = glm::vec3(
    //                          sin(m_horizontalAngle + PI / 2.f) * PI * m_speed,
    //                          0,
    //                          cos(m_horizontalAngle + PI / 2.f) * PI * m_speed
    //                          );

}

void Camera::rotateHorizontal(float diff)
{
    m_update = 1;
    m_view -= m_eye;
    m_view = glm::rotate(m_view, 2 * PI * m_sensitivity * diff, glm::vec3(0.f, 1.f, 0.f));
    m_view += m_eye;
//    m_horizontalAngle -= diff;
//    m_direction = {-startPosition[0], -startPosition[1], -startPosition[2] };

//    m_direction *= glm::vec3(
//                     cos(m_horizontalAngle),
//                     sin(m_verticalAngle),
//                     sin(m_horizontalAngle)
//                     );
}

void Camera::rotateVertical(float diff)
{
    m_update = 1;
    m_view -= m_eye;
    m_view -= glm::vec3(m_view.x, 0.f, m_view.z) * diff * m_speed * m_sensitivity;
    m_view += m_eye;
}

void Camera::moveX(float diff)
{
    m_update = 1;
    glm::vec3 dir =  m_view - m_eye;
    dir = glm::normalize(dir);
//    m_view -= m_eye;
    glm::vec3  tmp = glm::vec3(dir.x, 0.f, dir.z) * diff * m_speed;
    m_eye += tmp;
    m_view += tmp;
//    m_eye = m_view -m_eye;
}
void Camera::moveY(float diff)
{
    m_update = 1;
//    static glm::vec3 right = glm::vec3(
//                          sin(m_horizontalAngle + PI / 2.f) * PI * m_speed,
//                          0,
//                          cos(m_horizontalAngle + PI / 2.f) * PI * m_speed
//                          );
//    m_position += right * diff * m_speed;

}

void Camera::moveZ(float diff)
{
    m_update = 1;
//    m_up = glm::vec3(0.f, m_speed, 0.f) * glm::vec3(0, sin(m_verticalAngle), 0);
//    m_position += m_up * diff * m_speed * PI;
}

glm::mat4 Camera::view() const
{
    return m_viewMat;
}

glm::mat4 Camera::projection() const
{
    return m_projectionMat;
}

//glm::vec3 Camera::position() const
//{
//    return m_position;
//}

//glm::vec3 Camera::direction() const
//{
//    return m_direction;
//}

void Camera::update(Program &program)
{
    program.setViewMatrix(m_viewMat);
    program.setProjectionMatrix(m_projectionMat);
    program.setQuads(m_quads);
//    program.setUniform(glsl_view_matrix, m_view);
//    program.setUniform(glsl_projection_matrix, m_projection);
}
