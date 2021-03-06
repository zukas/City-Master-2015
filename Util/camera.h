#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera {
    static constexpr float max_distance{5000.f};

  private:
    glm::mat4 m_viewMat;
    glm::mat4 m_projectionMat;
	glm::vec3 m_eye{10.f, 10.f, 100.f};
	glm::vec3 m_view{400.f, 20.f, 1.f};
    glm::vec3 m_up{0.f, 1.f, 0.f};
    float m_velocity{0.5f};

  public:
    Camera();
    void update();
    void rotateHorizontal(float delta);
    void rotateVertical(float delta);
    void move(float delta);
    void strafe(float delta);
    void zoom(float delta);
    const glm::mat4 &view() const;
    const glm::mat4 &projection() const;
    const glm::vec3 &position() const;
    float distance() const;
};

#endif // CAMERA_H
