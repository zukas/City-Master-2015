#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "clock.h"

Camera::Camera(GLFWwindow *window) :
    m_window(window)
{
    m_viewMat = glm::lookAt(
                    m_eye,
                    m_view,
                    m_up);

    m_projectionMat = glm::perspective(45.f, 16.0f / 9.0f, 0.001f, 200.0f);
}

void Camera::calcViewport()
{
    if(m_window == nullptr) return;

    static double lastTime = glfwGetTime();
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);
    lastTime = currentTime;


    // Get view direction
    glm::vec3 vMove = m_view-m_eye;
    vMove = glm::normalize(vMove);
    vMove *= m_speed;

    glm::vec3 vStrafe = glm::cross(m_view-m_eye, m_up);
    vStrafe = glm::normalize(vStrafe);
    vStrafe *= m_speed;
    glm::vec3 vMoveBy;
    // Get vector of move


    if (glfwGetKey( m_window, GLFW_KEY_UP ) == GLFW_PRESS)
    {
        vMoveBy += vMove*deltaTime;
    }
    else if (glfwGetKey( m_window, GLFW_KEY_DOWN ) == GLFW_PRESS)
    {
        vMoveBy -= vMove*deltaTime;
    }

    if (glfwGetKey( m_window, GLFW_KEY_RIGHT ) == GLFW_PRESS)
    {
        vMoveBy += vStrafe*deltaTime;
    }
    else if (glfwGetKey( m_window, GLFW_KEY_LEFT ) == GLFW_PRESS)
    {
        vMoveBy -= vStrafe*deltaTime;
    }


    m_eye += vMoveBy;
    m_view += vMoveBy;


    if (glfwGetKey( m_window, GLFW_KEY_A ) == GLFW_PRESS)
    {
        m_view -= m_eye;
        m_view = glm::rotate(m_view, 2 * PI * m_sensitivity * deltaTime, glm::vec3(0.f, 1.f, 0.f));
        m_view += m_eye;
    }
    else if (glfwGetKey( m_window, GLFW_KEY_D ) == GLFW_PRESS)
    {
        m_view -= m_eye;
        m_view = glm::rotate(m_view, -2 * PI * m_sensitivity * deltaTime, glm::vec3(0.f, 1.f, 0.f));
        m_view += m_eye;
    }

    m_viewMat = glm::lookAt(m_eye, m_view, m_up);
    glm::vec3 tmp = m_view-m_eye;
    tmp = glm::normalize(tmp);
    m_quads[0] = glm::cross(tmp, m_up);
    m_quads[0] = glm::normalize(m_quads[0]);
    m_quads[1] = glm::cross(tmp, m_quads[0]);
    m_quads[1] = glm::normalize(m_quads[1]);

    //    glm::vec3 direction(-startPosition[0], -startPosition[1], -startPosition[2] );

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

    //    glm::vec3 up = glm::vec3(0.f, m_speed, 0.f) * glm::vec3(0, sin(m_verticalAngle), 0);



    //    if (glfwGetKey( m_window, GLFW_KEY_D ) == GLFW_PRESS){
    //        m_rotationMat = glm::rotate(m_rotationMat, -PI / 5 * deltaTime * m_speed, glm::vec3(0.f, 1.f, 0.f));
    //    }
    //    else if (glfwGetKey( m_window, GLFW_KEY_A ) == GLFW_PRESS){
    //        m_rotationMat = glm::rotate(m_rotationMat, PI / 5 * deltaTime * m_speed, glm::vec3(0.f, 1.f, 0.f));
    //    }

    ////    if (glfwGetKey( m_window, GLFW_KEY_W ) == GLFW_PRESS){
    ////       m_verticalAngle -= PI / 5 * deltaTime * m_speed;
    ////       if(m_verticalAngle < 0.f)
    ////       {
    ////           m_verticalAngle = 0.f;
    ////       }
    ////    }
    ////    else if (glfwGetKey( m_window, GLFW_KEY_S ) == GLFW_PRESS){
    ////        m_verticalAngle += PI / 5 * deltaTime * m_speed;
    ////        if(m_verticalAngle > 1.f)
    ////        {
    ////            m_verticalAngle = 1.f;
    ////        }
    ////    }




    //    if(glfwGetKey(m_window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
    //    {
    //        m_position += up * deltaTime * m_speed * PI;
    //    }
    //    else if(glfwGetKey(m_window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
    //    {
    //        m_position -= up * deltaTime * m_speed * PI;
    //    }

    //    if(m_position.y < 5.f)
    //    {
    //        m_position.y = 5.f;
    //    } else if(m_position.y > 80.f)
    //    {
    //        m_position.y = 80.f;
    //    }

    ////    m_position.
    ////    glm::distance(glm::vec3(0.f,0.f,0.f), m_position);
    ////    printf("Position: (%f, %f, %f)\n", m_position.x, m_position.y, m_position.z);

    ////    glm::vec4 vEye;
    ////    vEye *= m_rotationMat;
    ////    glm::vec3 vView = (m_position+direction);
    ////    vView *= m_rotationMat;
    ////    up *= m_rotationMat;

    //    glm::vec3 vEye = glm::vec3(m_rotationMat * glm::vec4(m_position, 1.f));
    //    glm::vec3 vView = glm::vec3(m_rotationMat * glm::vec4(m_position+direction, 0.f));
    //    glm::vec3 vUp = glm::vec3(m_rotationMat * glm::vec4(up, 0.f));


    //    m_viewMat = glm::lookAt(
    //                 vEye,           // Camera is here
    //                 vView, // and looks here : at the same position, plus "direction"
    //                 vUp                // Head is up (set to 0,-1,0 to look upside-down)
    //                 );


    //    m_direction = direction;
    //

    //    glm::vec3 tmp = glm::normalize(vView - vEye);
    //    m_quads[0] = glm::normalize(glm::cross(tmp, vUp));
    //    m_quads[1] = glm::normalize(glm::cross(tmp, m_quads[0]));

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
