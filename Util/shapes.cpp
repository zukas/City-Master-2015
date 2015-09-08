#include "shapes.h"
#include <glm/glm.hpp>



//SolidSphere(float radius, unsigned int rings, unsigned int sectors)
//	{
//		float const R = 1./(float)(rings-1);
//		float const S = 1./(float)(sectors-1);
//		int r, s;

//		vertices.resize(rings * sectors * 3);
//		normals.resize(rings * sectors * 3);
//		texcoords.resize(rings * sectors * 2);
//		std::vector<GLfloat>::iterator v = vertices.begin();
//		std::vector<GLfloat>::iterator n = normals.begin();
//		std::vector<GLfloat>::iterator t = texcoords.begin();
//		for(r = 0; r < rings; r++) for(s = 0; s < sectors; s++) {
//				float const y = sin( -M_PI_2 + M_PI * r * R );
//				float const x = cos(2*M_PI * s * S) * sin( M_PI * r * R );
//				float const z = sin(2*M_PI * s * S) * sin( M_PI * r * R );

//				*t++ = s*S;
//				*t++ = r*R;

//				*v++ = x * radius;
//				*v++ = y * radius;
//				*v++ = z * radius;

//				*n++ = x;
//				*n++ = y;
//				*n++ = z;
//		}

//		indices.resize(rings * sectors * 4);
//		std::vector<GLushort>::iterator i = indices.begin();
//		for(r = 0; r < rings-1; r++) for(s = 0; s < sectors-1; s++) {
//				*i++ = r * sectors + s;
//				*i++ = r * sectors + (s+1);
//				*i++ = (r+1) * sectors + (s+1);
//				*i++ = (r+1) * sectors + s;
//		}
//	}

void create2(uv_vertex *vertexes, uint32_t *indexes, float radius,
					uint32_t units) {

	const float RS = 1.f/float(units-1.f);
	uint32_t index = 0;
	for(uint32_t i = 0; i < units; ++i) {
		for(uint32_t j = 0; j < units; ++j) {
			const float y = glm::sin(-M_PI_2 + M_PI * float(i) * RS );
			const float x = glm::cos(2*M_PI * float(j) * RS) * glm::sin( M_PI * float(i) * RS );
			const float z = glm::sin(2*M_PI * float(j) * RS) * glm::sin( M_PI * float(i) * RS );

			vertexes[index++] = uv_vertex{glm::vec3{x, y, z} * radius, glm::vec3{x, y, z}, glm::vec2{float(j) * RS, float(i) * RS}};
		}
	}

	for(uint32_t i = 0, index = 0; i < units - 1; ++i) {
		for(uint32_t j = 0; j < units - 1; ++j) {
			indexes[index++] = i * units + j;
			indexes[index++] = i * units + (j + 1);
			indexes[index++] = (i + 1) * units + (j + 1);
			indexes[index++] = (i + 1) * units + j;
		}
	}
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

			float X = glm::cos(theta) * glm::sin(phi);
			float Y = glm::cos(phi);
			float Z = glm::sin(theta) * glm::sin(phi);

			vertexes[index++] = uv_vertex{glm::vec3{X, Y, Z} * radius, glm::vec3{0, 0, 0}, glm::vec2{U, V}};
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
	size *= 6;
	for(index = 0; index < size; ++index) {

		glm::vec3 t1 = vertexes[indexes[index]].p;
		glm::vec3 t2 = vertexes[indexes[(index + 1) % size]].p;
		glm::vec3 t3 = vertexes[indexes[(index + 2) % size]].p;

		glm::vec3 v1 = t2 - t1;
		glm::vec3 v2 = t3 - t1;

		glm::vec3 normal;

		normal.x = (v1.y * v2.z) - (v1.z * v2.y);
		normal.y = (v1.z * v2.x) - (v1.x * v2.z);
		normal.z = (v1.x * v2.y) - (v1.y * v2.x);


		vertexes[indexes[index]].n = normal;

	}
}
