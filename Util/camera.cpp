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
    m_right = glm::vec3(
                          sin(m_horizontalAngle + PI / 2.f) * PI * m_speed,
                          0,
                          cos(m_horizontalAngle + PI / 2.f) * PI * m_speed
                          );
    m_up = glm::vec3(0.f, m_speed, 0.f) * glm::vec3(0, sin(m_verticalAngle), 0);

}

void Camera::calcViewport()
{
    if(m_window == nullptr) return;

//    static double lastTime = glfwGetTime();
//    double currentTime = glfwGetTime();
//    float deltaTime = float(currentTime - lastTime);



//    glm::vec3 right = glm::vec3(
//                          sin(m_horizontalAngle + PI / 2.f) * PI * m_speed,
//                          0,
//                          cos(m_horizontalAngle + PI / 2.f) * PI * m_speed
//                          );




//    if (glfwGetKey( m_window, GLFW_KEY_D ) == GLFW_PRESS){
//        m_rotation = glm::rotate(m_rotation, -PI / 5 * deltaTime * m_speed, glm::vec3(0.f, 1.f, 0.f));
//    }
//    else if (glfwGetKey( m_window, GLFW_KEY_A ) == GLFW_PRESS){
//        m_rotation = glm::rotate(m_rotation, PI / 5 * deltaTime * m_speed, glm::vec3(0.f, 1.f, 0.f));
//    }

//    if (glfwGetKey( m_window, GLFW_KEY_W ) == GLFW_PRESS){
//       m_verticalAngle -= PI / 5 * deltaTime * m_speed;
//       if(m_verticalAngle < 0.f)
//       {
//           m_verticalAngle = 0.f;
//       }
//    }
//    else if (glfwGetKey( m_window, GLFW_KEY_S ) == GLFW_PRESS){
//        m_verticalAngle += PI / 5 * deltaTime * m_speed;
//        if(m_verticalAngle > 1.f)
//        {
//            m_verticalAngle = 1.f;
//        }
//    }


//    if (glfwGetKey( m_window, GLFW_KEY_UP ) == GLFW_PRESS){
//        m_position += glm::vec3(direction.x, 0.f, direction.z) * deltaTime * m_speed;
//    }
//    else if (glfwGetKey( m_window, GLFW_KEY_DOWN ) == GLFW_PRESS){
//        m_position -= glm::vec3(direction.x, 0.f, direction.z) * deltaTime * m_speed;
//    }

//    if (glfwGetKey( m_window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
//        m_position += right * deltaTime * m_speed;
//    }
//    else if (glfwGetKey( m_window, GLFW_KEY_LEFT ) == GLFW_PRESS){
//        m_position -= right * deltaTime * m_speed;
//    }

//    if(glfwGetKey(m_window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
//    {
//        m_position += up * deltaTime * m_speed * PI;
//    }
//    else if(glfwGetKey(m_window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
//    {
//        m_position -= up * deltaTime * m_speed * PI;
//    }

    if(m_position.y < 5.f)
    {
        m_position.y = 5.f;
    } else if(m_position.y > 80.f)
    {
        m_position.y = 80.f;
    }

//    m_position.
//    glm::distance(glm::vec3(0.f,0.f,0.f), m_position);
//    printf("Position: (%f, %f, %f)\n", m_position.x, m_position.y, m_position.z);


    m_view = glm::lookAt(
                 m_position,           // Camera is here
                 m_position+m_direction, // and looks here : at the same position, plus "direction"
                 std::move(m_up)                  // Head is up (set to 0,-1,0 to look upside-down)
                 ) * m_rotation;

}

void Camera::rotate(float diff)
{
        m_rotation = glm::rotate(m_rotation, PI / 5 * diff * m_speed, glm::vec3(0.f, 1.f, 0.f));
}

void Camera::moveX(float diff)
{
    m_direction = {-startPosition[0], -startPosition[1], -startPosition[2] };

    m_direction *= glm::vec3(
                     cos(m_horizontalAngle),
                     sin(m_verticalAngle),
                     sin(m_horizontalAngle)
                     );
    m_position += glm::vec3(m_direction.x, 0.f, m_direction.z) * diff * m_speed;
}
void Camera::moveY(float diff)
{
    m_right = glm::vec3(
                          sin(m_horizontalAngle + PI / 2.f) * PI * m_speed,
                          0,
                          cos(m_horizontalAngle + PI / 2.f) * PI * m_speed
                          );
    m_position += m_right * diff * m_speed;

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
