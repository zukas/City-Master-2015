#include "utils.h"
#include <stdio.h>
#include <glm/gtx/transform.hpp>

glm::mat4 scale_mat4(const glm::vec3 scale) {
	return {scale.x, 0.f, 0.f,     0.f, 0.f, scale.y, 0.f, 0.f,
			0.f,     0.f, scale.z, 0.f, 0.f, 0.f,     0.f, 1.f};
}

glm::mat4 translate_mat4(const glm::vec3 translate) {
	return {1.f, 0.f, 0.f, translate.x, 0.f, 1.f, 0.f, translate.y,
			0.f, 0.f, 1.f, translate.z, 0.f, 0.f, 0.f, 1.f};
}

glm::mat4 transform_mat4(const glm::quat &rotation,
						 const glm::vec3 &translation, const glm::vec3 &scale,
						 const glm::mat4 &base) {
	return base *
		   ((glm::scale(glm::mat4(1.f), scale) * glm::mat4_cast(rotation)) *
			glm::translate(glm::mat4(1.f), translation));
}

void stringify(char *buffer, glm::mat4 mat) {
	sprintf(buffer, "%.3f %.3f %.3f %.3f\n"
					"%.3f %.3f %.3f %.3f\n"
					"%.3f %.3f %.3f %.3f\n"
					"%.3f %.3f %.3f %.3f",
			mat[0][0], mat[0][1], mat[0][2], mat[0][3], mat[1][0], mat[1][1],
			mat[1][2], mat[1][3], mat[2][0], mat[2][1], mat[2][2], mat[2][3],
			mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
}

void stringify(char *buffer, glm::vec3 vec) {
	sprintf(buffer, "%.3f %.3f %.3f", vec.x, vec.y, vec.z);
}
