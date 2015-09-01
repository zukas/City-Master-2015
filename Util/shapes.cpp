#include "shapes.h"
#include <stdio.h>

shape sphare::create(float radius, int slices, int stacks) {
	shape res;
	res.data.reserve(slices * stacks + slices);
	res.indexes.reserve(slices * stacks + slices);

	for (float i = 0; i <= stacks; ++i) {
		// V texture coordinate.
		float V = i / (float)stacks;
		float phi = V * PI;

		for (float j = 0; j <= slices; ++j) {
			// U texture coordinate.
			float U = j / (float)slices;
			float theta = U * PI2;

			float X = cos(theta) * sin(phi);
			float Y = cos(phi);
			float Z = sin(theta) * sin(phi);
			res.data.push_back(
				uv_vertex{vec3{X, Y, Z} * radius, vec3{X, Y, Z}, vec2{U, V}});
		}
	}

	for (int i = 0, length = slices * stacks + slices; i < length; ++i) {
		res.indexes.push_back(i);
		res.indexes.push_back(i + slices + 1);
		res.indexes.push_back(i + slices);

		res.indexes.push_back(i + slices + 1);
		res.indexes.push_back(i);
		res.indexes.push_back(i + 1);
	}
	return res;
}

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

            float X = cos(theta) * sin(phi);
            float Y = cos(phi);
            float Z = sin(theta) * sin(phi);
            uv_vertex val{vec3{X, Y, Z} * radius, vec3{X, Y, Z}, vec2{U, V}};

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
