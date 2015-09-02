#include "shapes.h"
#include <glm/glm.hpp>

void sphare::create(uv_vertex *vertexes, uint32_t *indexes, float radius,
                    uint32_t units) {

    float _units = float(int32_t(units));
	uint32_t index = 0;
	uint32_t size = units * units + units;

    for (float i = 0; i <= units; ++i) {
        // V texture coordinate.
		float V = i / _units;
        float phi = V * PI;

        for (float j = 0; j <= units; ++j) {
            // U texture coordinate.
			float U = j / _units;
            float theta = U * PI2;

            float X = glm::cos(theta) * glm::sin(phi);
            float Y = glm::cos(phi);
            float Z = glm::sin(theta) * glm::sin(phi);
            uv_vertex val{glm::vec3{X, Y, Z} * radius, glm::vec3{X, Y, Z}, glm::vec2{U, V}};

            vertexes[index++] = val;
        }
    }

    for (index = 0; index < size; ++index) {
        indexes[index * 6] = index;
        indexes[index * 6 + 1] = index + units + 1;
        indexes[index * 6 + 2] = index + units;
        indexes[index * 6 + 3] = index + units + 1;
        indexes[index * 6 + 4] = index;
        indexes[index * 6 + 5] = index + 1;
    }
}
