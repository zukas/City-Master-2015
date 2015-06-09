#ifndef UTILS_H
#define UTILS_H

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>


glm::mat4 scale_mat4(const glm::vec3 scale);

glm::mat4 translate_mat4(const glm::vec3 translate);


glm::mat4 transform_mat4(const glm::quat &rotation,
						 const glm::vec3 &translation,
						 const glm::vec3 &scale,
						 const glm::mat4 &base = glm::mat4(1.f));

void stringify(char *buffer, glm::mat4 mat);
void stringify(char *buffer, glm::vec3 vec);

#endif // UTILS_H
