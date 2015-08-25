#include "glprogram.h"
#include "Memory/bitsmemory.h"
#include "Memory/utils.h"

#include <GL/glew.h>
#include <algorithm>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_interpolation.hpp>
#include <glm/gtx/transform.hpp>
//#include "Import/loader.h"
#include "Util/clock.h"
#include "Util/shapes.h"
#include "Util/utils.h"
#include "Util/gldebugger.h"
#include "Util/profiler.h"
#include "Util/texture2d.h"
#include "Util/utils.h"
#include "Util/viewport.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "resources/resourcemanager.h"
#include "Util/texture2dcollection.h"

#include <chrono>

constexpr float year{30 * 60 * 1000.f};
constexpr float size_div{6371.f};
constexpr float distance_div{14959.78707f};
constexpr float rat{distance_div / size_div};
constexpr float days_in_year{365.256f};
constexpr float day{year / days_in_year};
constexpr float hour{day / 23.9344722f};
constexpr float degree{0.0174532925f};

// glm::mat4 rot(const glm::mat4 &parent, )

struct planet_data {
	float orbiral_speed;
	float rotation_speed;
	glm::vec3 axis_tilt;
};

glm::mat4 __preproc_motion(const transform &t, model_data data) {

	float current = Clock::getDuration();
	planet_data *info = static_cast<planet_data *>(data);
	glm::mat4 mat = t;
	glm::mat4 rot = info->orbiral_speed > 0.f
						? glm::mat4_cast(glm::angleAxis(
							  PI2 * (current / info->orbiral_speed),
							  glm::vec3(0.f, 1.f, 0.f)))
						: glm::mat4(1.f);
	glm::mat4 tilt = glm::inverse(rot) * glm::eulerAngleYXZ(info->axis_tilt.y,
															info->axis_tilt.x,
															info->axis_tilt.z);

	glm::mat4 res = rot * mat * tilt;
	return res;
}

glm::mat4 __postproc_motion(const glm::mat4 &t, model_data data) {
	float current = Clock::getDuration();
	planet_data *info = static_cast<planet_data *>(data);
	glm::mat4 res = t * (info->rotation_speed > 0.f
							 ? glm::mat4_cast(glm::angleAxis(
								   PI2 * (current / info->rotation_speed),
								   glm::vec3(0.f, 1.f, 0.f)))
							 : glm::mat4(1.f));
	return res;
}

float system_size{143.73f * pow(10, 9)};
float system_scale{system_size / 50000000.f};

float size_scaling(float amp, float x) {
	return std::sqrt((x / (amp * 2)) + std::sqrt(amp / 25));
}

float distance_scaling(float amp, float x) {
	return std::sqrt((x / (amp * 2)) + std::sqrt(amp / 25));
}

struct buffer {
	unsigned char *data;
	size_t size;
};

enum PlanetType { SUN, PLANET, MOON, SHALLOW_MOON };

class Planet {
  public:
	const char *name;
	float size;
	float distance;
	float orbital_period;
	float rotation_period;
	glm::vec3 axis_tilt;
	PlanetType type;
    resource res;

	float calc_size;
	float calc_distance;
	int children_count{0};
	Planet **children{nullptr};

  public:
	void setup(const char *_name, float _size, float _distance,
			   float _orbtal_period, float _rotation_period,
			   glm::vec3 _axis_tilt, PlanetType _type, resource _res);
	void setChildrenCount(int size);
	void setChild(int pos, Planet *chiled);
	void calc();
	Model generate(float parent_size = 0.f);
};

void Planet::setup(const char *_name, float _size, float _distance,
                   float _orbtal_period, float _rotation_period,
				   glm::vec3 _axis_tilt, PlanetType _type, resource _res) {
	name = _name;
	size = _size;
	distance = _distance;
	orbital_period = _orbtal_period;
	rotation_period = _rotation_period;
	axis_tilt = _axis_tilt;
	type = _type;
    res = _res;
}

void Planet::setChildrenCount(int size) {
	children_count = size;
	children = new Planet *[size];
}

void Planet::setChild(int pos, Planet *chiled) { children[pos] = chiled; }

void Planet::calc() {
	switch (type) {
	case SUN:
		calc_size = size_scaling(115.f, size);
		calc_distance =
			distance > 0.f ? distance_scaling(100000.f, distance) : 0.f;
		break;
	case PLANET:
		calc_size = size_scaling(40.f, size);
		calc_distance = distance_scaling(1000.f, distance);
		break;
	case MOON:
	case SHALLOW_MOON:
		calc_size = size_scaling(255.f, size);
		calc_distance = distance_scaling(2000.f, distance);
		break;
	default:
		break;
	}
}

Model Planet::generate(float parent_size) {
	calc();
	float parts;
	switch (type) {
	case SUN:
		parts = 256;
		break;
	case PLANET:
		parts = 256;
		break;
	case MOON:
		parts = 128;
		break;
	case SHALLOW_MOON:
		parts = 16;
		break;
	default:
		parts = 4;
		break;
	}

    auto d = sphare::create(calc_size / 2.f, parts, parts);

    Mesh3D m{d.data.data(), d.data.size(), d.indexes.data(), d.indexes.size()};
	Texture2D tex;
	tex.init_dds_from_memory(res.buffer, res.size);
	m.texture(0) = tex.move();

	Model mod{new Mesh3D[1]{std::move(m)}, 1};
	mod.setLocation(
		{parent_size + (calc_distance > 0.f ? calc_size + calc_distance : 0.f),
		 0.f, 0.f});
	mod.setModelData(
		new planet_data{orbital_period, rotation_period, axis_tilt});
	mod.setModelPreProcFunction(__preproc_motion);
	mod.setModelPostProcFunction(__postproc_motion);

	for (int i = 0; i < children_count; ++i) {
		mod.addChild(
			children[i]->generate(type == SUN ? calc_size * 2 : calc_size));
	}

	return mod;
}

glProgram::glProgram() {

	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		exit(1);
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, nullptr,
								nullptr);
	glfwMakeContextCurrent(m_window);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		exit(1);
	} else {
		if (!GLEW_VERSION_3_3) {
			fprintf(stderr, "OpenGL 3.3 is not supported\n");
			exit(1);
		}
	}

	glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetCursorPos(m_window, halfWindowWidth, halfWindowHeight);
	m_camera = {m_window};
	m_lamp = {glm::vec3(0.f, 0.f, 5.f), 500.f};

	Viewport::update({0, 0, windowWidth, windowHeight});

	glClearColor(0.05f, 0.075f, 0.1f, 1.0f);

	glClearDepth(1.0f);
	glDepthFunc(GL_LESS);

	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	init_resources();

	m_objectProgram.createShader({VERTEX, get_res_object_vert_glsl()});
	m_objectProgram.createShader({FRAGMENT, get_res_object_frag_glsl()});
    m_objectProgram.createProgram();
    m_objectProgram.linkProgram();
    m_objectProgram.resolveUniforms();

//	Shader ss[2]{{VERTEX, get_res_object_vert_glsl()},
//				 {FRAGMENT, get_res_object_frag_glsl()}};

//	uint32_t pid = ProgramCompiler::compileProgram(ss, 2);

//	uint32_t uni[6];
//	ProgramCompiler::resolveUniforms(pid, uni, 6);

//	uint32_t uni2 = crypto::sid("modelMatrix");

    m_objectSelectionProgram.createShader(
        {VERTEX, get_res_object_select_vert_glsl()});
    m_objectSelectionProgram.createShader(
        {FRAGMENT, get_res_object_select_frag_glsl()});
    m_objectSelectionProgram.createProgram();
    m_objectSelectionProgram.linkProgram();
    m_objectSelectionProgram.resolveUniforms();

    m_axis.init();

    float mercury_distance = 57910000.f;
    float venus_distance = 108200000.f;
    float earth_distance = 149597870.691f;
    float moon_distance = 384400.f;
    float mars_distance = 227900000.f;
    float jupiter_distance = 778500000.f;
    float europa_distance = 670900.f;
    float io_distance = 421700.f;
    float callisto_distance = 1882700.f;
    float ganymede_distance = 1070400.f;
    float saturn_distance = 1433000000.f;

    float sun_size = 696000.f;
    float mercury_size = 2440.f;
    float venus_size = 6052.f;
    float earth_size = 6371.f;
    float moon_size = 1737.1f;
    float mars_size = 3390.f;
    float jupiter_size = 69911.f;
    float europa_size = 1560.8f;
    float io_size = 1821.6f;
    float callisto_size = 2410.3f;
    float ganymede_size = 2634.1f;
    float saturn_size = 58232.f;

    BitsMemory::init(1024 * 1024 * 20);
    set_malloc_function(BitsMemory::malloc_aligend_64);

    Planet sun;
    sun.setup("sun", sun_size, 0.f, 0.f, 25.6f * day,
			  {0.f, 0.f, 0.f /*7.25f * degree*/}, SUN, get_res_sun_dds());

    Planet mercury;
    mercury.setup("mercury", mercury_size, mercury_distance, day * 88.f,
                  day * 58.6f, {0.f, 0.f, 0.1 * degree}, PLANET,
                  get_res_mercury_dds());

    Planet venus;
    venus.setup("venus", venus_size, venus_distance, day * 224.701f,
                day * 116.75f, {0.f, 0.f, 177.f * degree}, PLANET,
                get_res_venus_dds());

    Planet earth;
    earth.setup("earth", earth_size, earth_distance, year, day,
				{0.f, 0.f, 23.f * degree}, PLANET, get_res_earth_dds());

    Planet moon;
    moon.setup("moon", moon_size, moon_distance, day * 29.530589f, 0.f,
			   {0.f, 0.f, 6.687f * degree}, MOON, get_res_moon_dds());

    earth.setChildrenCount(1);
    earth.setChild(0, &moon);

    Planet mars;
    mars.setup("mars", mars_size, mars_distance, 686.971f * day,
               1.02876421707f * day, {0.f, 0.f, 25.f * degree}, PLANET,
               get_res_mars_dds());

    Planet jupiter;
    jupiter.setup("jupiter", jupiter_size, jupiter_distance, 11.86f * year,
                  9.97f * hour, {0.f, 0.f, 3.f * degree}, PLANET,
                  get_res_jupiter_dds());

    Planet europa;
    europa.setup("europa", europa_size, europa_distance, 3.551181f * day, 0.f,
				 {0.f, 0.f, 0.1f * degree}, MOON, get_res_europa_dds());

    Planet io;
    io.setup("io", io_size, io_distance, 1.769137786f * day, 0.f,
             {0.f, 0.f, 0.05f * degree}, MOON, get_res_io_dds());

    Planet ganymede;
    ganymede.setup("ganymede", ganymede_size, ganymede_distance,
                   7.15455296f * day, 0.f, {0.f, 0.f, 2.214f * degree}, MOON,
                   get_res_ganymede_dds());

    Planet callisto;
    callisto.setup("callisto", callisto_size, callisto_distance,
                   16.6890184f * day, 0.f, {0.f, 0.f, 2.017f * degree}, MOON,
                   get_res_callisto_dds());

    jupiter.setChildrenCount(4);
    jupiter.setChild(0, &europa);
    jupiter.setChild(1, &io);
    jupiter.setChild(2, &ganymede);
    jupiter.setChild(3, &callisto);

    Planet saturn;
    saturn.setup("saturn", saturn_size, saturn_distance, 29.f * year,
                 10.65f * hour, {0.f, 0.f, 26.7f * degree}, PLANET,
                 get_res_saturn_dds());

    Planet titan;
    titan.setup("titan", 2576.f, 1221870.f, 15.945f * day, 0.f,
				{0.f, 0.f, 0.f * degree}, MOON, get_res_titan_dds());

    Planet enceladus;
    enceladus.setup("enceladus", 252.f, 238020.f, 1.370218f * day, 0.f,
					{0.f, 0.f, 0.017f * degree}, MOON, get_res_enceladus_dds());

    Planet rhea;
    rhea.setup("rhea", 763.8f, 527108.f, 4.518212f * day, 0.f,
               {0.f, 0.f, 0.f * degree}, MOON, get_res_rhea_dds());

    Planet dione;
    dione.setup("dione", 561.4f, 377400.f, 2.736915f * day, 0.f,
				{0.f, 0.f, 0.f * degree}, MOON, get_res_dione_dds());

    Planet tethys;
    tethys.setup("tethys", 531.1f, 294619.f, 1.887802f * day, 0.f,
				 {0.f, 0.f, 0.f * degree}, MOON, get_res_tethys_dds());

    Planet iapetus;
    iapetus.setup("iapetus", 734.5f, 3560820.f, 79.3215f * day, 0.f,
				  {0.f, 0.f, 0.f * degree}, MOON, get_res_iapetus_dds());

    Planet mimas;
    mimas.setup("mimas", 198.2f, 185539.f, 0.942f * day, 0.f,
				{0.f, 0.f, 0.f * degree}, MOON, get_res_mimas_dds());

    saturn.setChildrenCount(7);
    saturn.setChild(0, &titan);
    saturn.setChild(1, &enceladus);
    saturn.setChild(2, &rhea);
    saturn.setChild(3, &dione);
    saturn.setChild(4, &tethys);
    saturn.setChild(5, &iapetus);
    saturn.setChild(6, &mimas);

    Planet uranus;
    uranus.setup("uranus", 25362.f, 2870671400.f, 84.016846f * year,
                 17.233333333f * hour, {0.f, 0.f, 97.77f * degree}, PLANET,
                 get_res_uranus_dds());

    Planet neptune;
    neptune.setup("neptune", 24622.f, 4503000000.f, 165 * year,
                  16.266666667f * hour, {0.f, 0.f, 28.32f * degree}, PLANET,
                  get_res_neptune_dds());

    sun.setChildrenCount(8);
    sun.setChild(0, &mercury);
    sun.setChild(1, &venus);
    sun.setChild(2, &earth);
    sun.setChild(3, &mars);
    sun.setChild(4, &jupiter);
    sun.setChild(5, &saturn);
    sun.setChild(6, &uranus);
    sun.setChild(7, &neptune);

    m_models.push_back(sun.generate());

	BitsMemory::clear();

	auto dd = get_res_neptune_dds();
	Texture2D tt;
	tt.init_dds_from_memory(dd.buffer, dd.size);

	BitsMemory::clear();

	{
		Shader text_[2]{{VERTEX, get_res_text_vert_glsl()},
						{FRAGMENT, get_res_text_frag_glsl()}};

		Text::init(text_, 2, "projectionMatrix"_h, "modelMatrix"_h, "colour"_h,
				   "texture"_h);
	}

	Texture2DCollection::init(256);

    auto freesans_buffer = get_res_freesans_ttf();
	m_text = {freesans_buffer.buffer, freesans_buffer.size, 32};
	m_text.setColour(colour(150, 160, 170, 255));
//    glDebugger::init({freesans_buffer.buffer, freesans_buffer.size, 14});

	BitsMemory::deinit();

	deinit_resources();

	m_mouse = {m_window};
	m_keyboard = {m_window};

	m_mouse.onClickLeft(std::bind(&glProgram::handleSelection, this,
								  std::placeholders::_1,
								  std::placeholders::_2));

	m_mouse.onDragLeft([=](MoveDirection event, float diff) {
		if (event == MOVE_Y) {
			m_camera.move(diff);
		} else if (event == MOVE_X) {
			m_camera.strafe(diff);
		} else if (event == MOVE_Z) {
			m_camera.zoom(diff);
		}
	});

	m_mouse.onDragRight([=](MoveDirection event, float diff) {
		if (event == MOVE_Y) {
			m_camera.rotateVertical(diff);
		} else if (event == MOVE_X) {
			m_camera.rotateHorizontal(diff);
		} else if (event == MOVE_Z) {
			m_camera.zoom(diff);
		}
	});

	m_keyboard.moveX([=](float diff) { m_camera.move(diff); });

	m_keyboard.moveY([=](float diff) { m_camera.strafe(diff); });

	m_keyboard.moveZ([=](float diff) { m_camera.zoom(diff); });

	m_keyboard.rotateHorizontal(
		[=](float diff) { m_camera.rotateHorizontal(diff); });

	m_keyboard.rotateVertical(
		[=](float diff) { m_camera.rotateVertical(diff); });

    glfwSwapInterval(1);
}

void glProgram::exec() {
	int frameCount = 0;
	auto startTime = std::chrono::high_resolution_clock::now();
	do {
		m_mouse.update();
		m_keyboard.update();
		render();
		glfwPollEvents();
		++frameCount;
		m_frameRate = frameCount /
					  (std::chrono::duration_cast<std::chrono::duration<float>>(
						   std::chrono::high_resolution_clock::now() -
						   startTime).count());
		if (frameCount == 600) {
			startTime = std::chrono::high_resolution_clock::now();
			frameCount = 0;
		}

	} while (glfwGetKey(m_window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
			 glfwWindowShouldClose(m_window) == 0);
}

void glProgram::render() {
    PROF;
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        PROF;
        Clock::update();
		m_camera.calcViewport();

        m_objectProgram.use();
        m_lamp.update(m_objectProgram);
        m_camera.update(m_objectProgram);

        {
            PROF;
            for (auto &m : m_models) {
                m.render(m_objectProgram);
            }
        }

		Text::beginRender();
        char buf[128];
        std::sprintf(buf, "FPS: %.2f", m_frameRate);
		m_text.render(buf, 20, 40);
//		glDebugger::flush();
		Text::endRender();
    }

	glfwSwapBuffers(m_window);
}

void glProgram::handleSelection(double x, double y) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_objectSelectionProgram.use();
	m_camera.update(m_objectSelectionProgram);

	for (auto &m : m_models) {
		m.render(m_objectSelectionProgram);
	}

	unsigned char res[4]{0};

	double viewportheight = m_objectSelectionProgram.getViewport().h;
	double screenX = x;
	double screenY = viewportheight - y;
	glReadPixels(screenX, screenY, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, res);

	auto it = std::find(std::begin(m_models), std::end(m_models),
						ColourID(res[0], res[1], res[2]).value());
	if (it != std::end(m_models)) {
		it->select(!it->selected());
	}
}
