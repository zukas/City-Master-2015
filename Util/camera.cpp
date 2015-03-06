#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "clock.h"

Camera::Camera(GLFWwindow *window) :
    m_window(window)
{
    m_view = glm::lookAt(
    m_position,
    glm::vec3 { 0, 0, 0 },
    glm::vec3 { 0, 3, 0 });
    m_projection = glm::perspective(45.f, 16.0f / 9.0f, 0.0001f, 1000.0f);
}

void Camera::calcViewport()
{
    if(m_window == nullptr) return;

    static double lastTime = glfwGetTime();
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);

    glm::vec3 direction(-startPosition[0], -startPosition[1], -startPosition[2] );

    direction *= glm::vec3(
                     cos(m_horizontalAngle),
                     sin(m_verticalAngle),
                     sin(m_horizontalAngle)
                     );

    glm::vec3 right = glm::vec3(
                          sin(m_horizontalAngle + PI / 2.f) * m_speed,
                          0,
                          cos(m_horizontalAngle + PI / 2.f) * m_speed
                          );

    glm::vec3 up = glm::vec3(0.f, m_speed, 0.f) * glm::vec3(0, sin(m_verticalAngle), 0);



    if (glfwGetKey( m_window, GLFW_KEY_D ) == GLFW_PRESS){
        m_rotation = glm::rotate(m_rotation, -PI / 5 * deltaTime * m_speed, glm::vec3(0.f, 1.f, 0.f));
    }
    else if (glfwGetKey( m_window, GLFW_KEY_A ) == GLFW_PRESS){
        m_rotation = glm::rotate(m_rotation, PI / 5 * deltaTime * m_speed, glm::vec3(0.f, 1.f, 0.f));
    }

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


    if (glfwGetKey( m_window, GLFW_KEY_UP ) == GLFW_PRESS){
        m_position += glm::vec3(direction.x, 0.f, direction.z) * deltaTime * m_speed;
    }
    else if (glfwGetKey( m_window, GLFW_KEY_DOWN ) == GLFW_PRESS){
        m_position -= glm::vec3(direction.x, 0.f, direction.z) * deltaTime * m_speed;
    }

    if (glfwGetKey( m_window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
        m_position += right * deltaTime * m_speed;
    }
    else if (glfwGetKey( m_window, GLFW_KEY_LEFT ) == GLFW_PRESS){
        m_position -= right * deltaTime * m_speed;
    }

    if(glfwGetKey(m_window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
    {
        m_position += up * deltaTime * m_speed;
    }
    else if(glfwGetKey(m_window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
    {
        m_position -= up * deltaTime * m_speed;
    }

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
                 m_position+direction, // and looks here : at the same position, plus "direction"
                 std::move(up)                  // Head is up (set to 0,-1,0 to look upside-down)
                 ) * m_rotation;



    lastTime = currentTime;
}

glm::mat4 Camera::viewport() const
{
    return m_view;
}

glm::mat4 Camera::projection() const
{
    return m_projection;
}

void Camera::update(Program &program)
{
    program.setUniform(glsl_view_matrix, m_view);
    program.setUniform(glsl_projection_matrix, m_projection);
}
