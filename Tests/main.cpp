#include "glprogram.h"
#include "Util/mesh3d.h"
#include "Util/mesh.h"
#include "Util/model.h"
#include "Util/profiler.h"

#include <glm/gtx/euler_angles.hpp>

constexpr float year{5 * 60 * 1000.f};
constexpr float days_in_year{365.256f};
constexpr float day{year / days_in_year};
constexpr float degree{0.0174532925f};

void stringify(const glm::quat &quat) {
	printf("%4.4f %4.4f %4.4f %4.4f\n\n", quat.x, quat.y, quat.z, quat.w);
}

glm::quat rot_quat_t(float h_ang) {
	return glm::quat{glm::cos(h_ang), 0.f, glm::sin(h_ang), 0.f};
}

glm::quat base_rot_t(float current, float val) {
	return val != 0 ? rot_quat_t(PI * (current / val)) : glm::quat();
}

void run_test() {

	float deg_25 = 25 * 0.0174532925f;
	float current = 15156.f;
	const glm::quat l_rot = base_rot_t(current, day);
	const glm::quat o_rot = base_rot_t(current, year);
	const glm::quat o_rot_i = -o_rot;
	const glm::quat b_t = glm::quat_cast(glm::eulerAngleZ(deg_25));
	const glm::quat tilt = o_rot_i * b_t;

	printf("deg_25: %f, current: %f, day: %f, yeah: %f\n\n", deg_25, current, day, year);

	stringify(l_rot);

	stringify(o_rot);

	stringify(o_rot_i);

	stringify(b_t);

	stringify(tilt);

}

int main() {

	run_test();
	return 0;

	glProgram p;
	p.exec();

    print_profiler_results();
    return 0;
}
