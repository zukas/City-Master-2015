#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "clock.h"
#include "utils.h"
#include "gldebugger.h"

// void Camera::calcQuads() {
//	glm::vec3 tmp = m_view - m_eye;
//	tmp = glm::normalize(tmp);
//	m_quads[0] = glm::cross(tmp, m_up);
//	m_quads[0] = glm::normalize(m_quads[0]);
//	m_quads[1] = glm::cross(tmp, m_quads[0]);
//	m_quads[1] = glm::normalize(m_quads[1]);
//}

Camera::Camera() {
    m_viewMat = glm::lookAt(m_eye, m_view, m_up);

    m_projectionMat = glm::perspective(45.f, 16.0f / 9.0f, 0.005f, 5000.0f); //CONVERT TO CONSTEXPR
    //	calcQuads();
}

void Camera::update() {
    m_viewMat = glm::lookAt(m_eye, m_view, m_up); //no need to store here
    //    calcQuads();
}

void Camera::rotateHorizontal(float delta) {
    auto diff = m_view - m_eye;
    auto norm = glm::normalize(diff);
    auto dir = glm::cos(norm);

    m_view -= m_eye;
    auto offset = (m_view.y > 0 ? PI : -PI) * m_velocity * delta;
    m_view = glm::rotate(m_view, offset, glm::vec3(0.f, diff.y * dir.y, 0.f));
    m_view += m_eye;
}

void Camera::rotateVertical(float delta) {
    auto diff = m_view - m_eye;
    auto norm = glm::normalize(diff);
    auto dir = glm::cos(norm);
    auto tmp = delta;
    if (tmp > 0) {
        tmp = 0.05;
    } else {
        tmp = -0.05;
    }

    if (norm.y - tmp <= -1.f || norm.y - tmp >= 1.f) {
        return;
    }

    m_view -= m_eye;
    auto offset = -PI * m_velocity * delta;
    m_view = glm::rotate(
        m_view, offset,
        glm::cross(glm::vec3(diff.x * dir.x, 0.f, diff.z * dir.z), m_up));
    m_view += m_eye;
}

void Camera::move(float delta) {
    auto diff = m_view - m_eye;
    auto dir = glm::cos(glm::normalize(diff));
    auto tmp =
        glm::vec3(diff.x * dir.x, 0.f, diff.z * dir.z) * delta * m_velocity;
    auto plane = m_eye + tmp;
    if (glm::distance(glm::vec3(0, 0.f, 0), plane) >= max_distance) {
        return;
    }

    m_view += tmp;
    m_eye = std::move(plane);
}
void Camera::strafe(float delta) {
    auto diff = glm::cross(m_view - m_eye, m_up);
    auto dir = glm::cos(glm::normalize(diff));
    auto tmp =
        glm::vec3(diff.x * dir.x, 0.f, diff.z * dir.z) * delta * m_velocity;
    auto plane = m_eye + tmp;
    if (glm::distance(glm::vec3(0, 0.f, 0), plane) >= max_distance) {
        return;
    }

    m_view += tmp;
    m_eye = std::move(plane);
}

void Camera::zoom(float delta) {
    auto diff = m_view - m_eye;
    auto dir = glm::cos(glm::normalize(diff));

    auto tmp = (diff * dir) * delta * m_velocity;
    auto plane = m_eye + tmp;
    if (glm::distance(glm::vec3(0, 0.f, 0), plane) >= max_distance) {
        return;
    }

    m_view += tmp;
    m_eye = plane;
}

const glm::mat4 &Camera::view() const { return m_viewMat; }

const glm::mat4 &Camera::projection() const { return m_projectionMat; }

float Camera::distance() const {
    return glm::distance(glm::vec3(0.f, 0.f, 0.f), m_eye);
}

// void Camera::update(Program &program) {
//	program.setViewMatrix(m_viewMat);
//	program.setProjectionMatrix(m_projectionMat);
////	program.setQuads(m_quads);
//}
