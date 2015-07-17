#include "glprogram.h"
#include <GL/glew.h>
#include <algorithm>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_interpolation.hpp>
#include <glm/gtx/transform.hpp>
#include "loader.h"
#include "clock.h"
#include "shapes.h"
#include "utils.h"
#include "gldebugger.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "resources/res_object_frag_glsl.h"
#include "resources/res_object_vert_glsl.h"
#include "resources/res_object_select_frag_glsl.h"
#include "resources/res_object_select_vert_glsl.h"
#include "resources/res_freesans_ttf.h"

#include "resources/res_callisto_dds.h"
#include "resources/res_dione_dds.h"
#include "resources/res_earth_dds.h"
#include "resources/res_enceladus_dds.h"
#include "resources/res_europa_dds.h"
#include "resources/res_ganymede_dds.h"
#include "resources/res_iapetus_dds.h"
#include "resources/res_io_dds.h"
#include "resources/res_jupiter_dds.h"
#include "resources/res_mars_dds.h"
#include "resources/res_mercurymap_dds.h"
#include "resources/res_mimas_dds.h"
#include "resources/res_moon_dds.h"
#include "resources/res_neptune_dds.h"
#include "resources/res_rhea_dds.h"
#include "resources/res_saturn_dds.h"
#include "resources/res_sun_dds.h"
#include "resources/res_tethys_dds.h"
#include "resources/res_titan_dds.h"
#include "resources/res_uranus_dds.h"
#include "resources/res_venusmap_dds.h"

#include <chrono>

constexpr float year { 5 *	 60 * 1000.f };
constexpr float size_div { 6371.f };
constexpr float distance_div { 14959.78707f };
constexpr float rat { distance_div / size_div };
constexpr float days_in_year { 365.256f };
constexpr float day { year / days_in_year };
constexpr float hour { day / 23.9344722f };
constexpr float degree { 0.0174532925f };

//glm::mat4 rot(const glm::mat4 &parent, )

struct planet_data
{
		float orbiral_speed;
		float rotation_speed;
		glm::vec3 axis_tilt;
};


glm::mat4 __preproc_motion(const transform &t, model_data data)
{

	float current = Clock::getDuration();
	planet_data *info = static_cast<planet_data*>(data);
	glm::mat4 mat = t;
	glm::mat4 rot = info->orbiral_speed > 0.f ? glm::mat4_cast(glm::angleAxis(PI2 * (current / info->orbiral_speed), glm::vec3(0.f, 1.f, 0.f))) : glm::mat4(1.f);
	glm::mat4 tilt = glm::inverse(rot) * glm::eulerAngleYXZ(info->axis_tilt.y, info->axis_tilt.x, info->axis_tilt.z);

	glm::mat4 res = rot * mat * tilt;
	return  res;
}

glm::mat4 __postproc_motion(const glm::mat4 &t, model_data data)
{
	float current = Clock::getDuration();
	planet_data *info = static_cast<planet_data*>(data);
	glm::mat4 res = t * (info->rotation_speed > 0.f ? glm::mat4_cast(glm::angleAxis(PI2 * (current / info->rotation_speed), glm::vec3(0.f, 1.f, 0.f))) : glm::mat4(1.f));
	return res;
}


//float scaling(float amp, float distance)
//{
////	float pw = std::pow(distance, 2.f);

////	return (pw / std::exp(pw * 0.001f)) * amp * 750.f;

//	return std::pow(std::exp(amp / distance), 2.f) * std::log10(distance) * amp;
//}


constexpr float system_size =  143.73 * pow(10, 9);
constexpr float system_scale = system_size / 50000000.f;

float size_scaling(float amp, float x)
{
	return std::sqrt((x / (amp * 2) ) + std::sqrt(amp/25));
	//	return (std::sqrt(std::pow(x, 3) * std::log10(x)) /(10000.f * amp));
	//	return x / (1.f/100000000.f * std::sqrt(std::pow(x, 4)) + amp);
}

float distance_scaling(float amp, float x)
{
	return std::sqrt((x / (amp * 2) ) +  std::sqrt(amp/25));
	//return std::sqrt(std::pow(x, 4)/(125000000.f * amp)) / x;
}

enum PlanetType
{
	SUN,
	PLANET,
	MOON,
	SHALLOW_MOON
};

class Planet
{
	public:
		const char *name;
		float size;
		float distance;
		float orbital_period;
		float rotation_period;
		glm::vec3 axis_tilt;
		PlanetType type;
        unsigned char *texture_buffer;
        long int buffer_size;

		float calc_size;
		float calc_distance;
		int children_count { 0 };
		Planet **children { nullptr };

	public:
		void setup(const char *_name,
				   float _size,
				   float _distance,
				   float _orbtal_period,
				   float _rotation_period,
				   glm::vec3 _axis_tilt,
				   PlanetType _type,
                   unsigned char * _texture_buffer, long int _buffer_size);
		void setChildrenCount(int size);
		void setChild(int pos, Planet *chiled);
		void calc();
		Model generate(float parent_size = 0.f);
};

void Planet::setup(const char *_name, float _size, float _distance, float _orbtal_period, float _rotation_period, glm::vec3 _axis_tilt, PlanetType _type, unsigned char *_texture_buffer, long _buffer_size)
{
	name = _name;
	size = _size;
	distance = _distance;
	orbital_period = _orbtal_period;
	rotation_period = _rotation_period;
	axis_tilt = _axis_tilt;
	type = _type;
    texture_buffer = _texture_buffer;
    buffer_size = _buffer_size;
}

void Planet::setChildrenCount(int size)
{
	children_count = size;
	children = new Planet*[size];
}

void Planet::setChild(int pos, Planet *chiled)
{
	children[pos] = chiled;
}

void Planet::calc()
{
	switch (type) {
		case SUN:
			calc_size = size_scaling(115.f ,size);
			calc_distance = distance > 0.f ? distance_scaling(100000.f, distance) : 0.f;
			break;
		case PLANET:
			calc_size = size_scaling(40.f ,size);
			calc_distance = distance_scaling(1500.f, distance);
			break;
		case MOON:
		case SHALLOW_MOON:
			calc_size = size_scaling(155.f ,size);
			calc_distance = distance_scaling(8000.f, distance);
			break;
		default:
			break;
	}
}

Model Planet::generate(float parent_size)
{
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

	Mesh m { sphare::create(calc_size / 2.f, parts, parts) };
    m.addTexture({GL_TEXTURE_2D, texture_buffer, buffer_size});

	Model mod { { std::move(m) } };
	mod.setLocation({ parent_size + (calc_distance > 0.f ? calc_size + calc_distance : 0.f), 0.f, 0.f });
	mod.setModelData(new planet_data { orbital_period, rotation_period, axis_tilt });
	mod.setModelPreProcFunction(__preproc_motion);
	mod.setModelPostProcFunction(__postproc_motion);

	for(int i = 0; i < children_count; ++i)
	{
		mod.addChild(children[i]->generate(type == SUN ? calc_size * 2 : calc_size));
	}

	return mod;

}

glProgram::glProgram()
{

	if(!glfwInit())
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		exit(1);
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow( windowWidth, windowHeight, windowTitle, nullptr, nullptr);
	glfwMakeContextCurrent(m_window);

	glewExperimental = GL_TRUE;

	if(glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		exit(1);
	} else
	{
		if(!GLEW_VERSION_3_3)
		{
			fprintf(stderr, "OpenGL 3.3 is not supported\n");
			exit(1);
		}
	}


	glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetCursorPos(m_window, halfWindowWidth, halfWindowHeight);
	m_camera = { m_window };
	m_lamp = { glm::vec3(0.f, 0.f, 5.f), 500.f };

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

//	m_objectProgram = { "shaders/object_vert.glsl", "shaders/object_frag.glsl" };
	{
        m_objectProgram.createShader({ VERTEX,  get_res_object_vert_glsl() });
        m_objectProgram.createShader({ FRAGMENT,  get_res_object_frag_glsl() });
		m_objectProgram.createProgram();
		m_objectProgram.linkProgram();
		m_objectProgram.resolveUniforms();
	}

	{
        m_objectSelectionProgram.createShader({VERTEX, get_res_object_select_vert_glsl() });
        m_objectSelectionProgram.createShader({FRAGMENT, get_res_object_select_frag_glsl() });
		m_objectSelectionProgram.createProgram();
		m_objectSelectionProgram.linkProgram();
		m_objectSelectionProgram.resolveUniforms();
	}

//	m_objectSelectionProgram = {"shaders/object_select_vert.glsl", "shaders/object_select_frag.glsl", Selection };
//	m_axisProgram = { "shaders/basic_vert.glsl", "shaders/basic.frag" };
//	m_textProgram = { "shaders/text.vert", "shaders/text.frag" };
	//	m_skyProgram = { "shaders/skybox.vert", "shaders/skybox.frag" };

	m_axis.init();
	m_fire = new ParticleSystem;
//	Texture pt { GL_TEXTURE_2D, "textures/fire2.jpg"};
	//    pt.setSamplerParameter(GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
	//	m_fire->setTexture(pt);
	//	m_fire->setBlendFunc(GL_SRC_ALPHA, GL_ONE);


	//    m_smoke = new ParticleSystem;
	//    m_smoke->setTexture(std::move(pt));
	//    m_smoke->setBlendFunc(GL_ONE, GL_SRC_ALPHA);
	//    m_smoke->setProperties(
	//                glm::vec3(-2.f, 5.f, -2.f), // Where the particles are generated
	//                glm::vec3(0.0, -0.01, 0.0), // Minimal velocity
	//                glm::vec3(0.0, -0.005, 0.0), // Maximal velocity
	//                glm::vec3(0.0, -0.008, 0.0), // Gravity force applied to particles
	//                glm::vec3(1.f, 1.f, 1.f), // Color (light blue)
	//                2000.f, // Minimum lifetime in seconds
	//                2500.0f, // Maximum lifetime in seconds
	//                0.25f, // Rendered size
	//                6.f, // Spawn every 0.05 seconds
	//                30); // And spawn 30 particles

//	m_box =
//	{
//		"textures/bluecloud_lf.jpg",
//		"textures/bluecloud_rt.jpg",
//		"textures/bluecloud_dn.jpg",
//		"textures/bluecloud_up.jpg",
//		"textures/bluecloud_ft.jpg",
//		"textures/bluecloud_bk.jpg"
//	};


	//	float system =  143.73 * std::pow(10, 9);
	//	float system_scale = system / 50000000.f;
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

    auto sun_buff = get_res_sun_dds();
    auto mercury_buff = get_res_mercurymap_dds();
    auto venus_buff = get_res_venusmap_dds();
    auto earth_buff = get_res_earth_dds();
    auto moon_buff = get_res_moon_dds();
    auto mars_buff = get_res_mars_dds();
    auto jupiter_buff = get_res_jupiter_dds();
    auto europa_buff = get_res_europa_dds();
    auto io_buff = get_res_io_dds();
    auto ganymede_buff = get_res_ganymede_dds();
    auto callisto_buff = get_res_callisto_dds();
    auto saturn_buff = get_res_saturn_dds();
    auto titan_buff = get_res_titan_dds();
    auto enceladus_buff = get_res_enceladus_dds();
    auto rhea_buff = get_res_rhea_dds();
    auto dione_buff = get_res_dione_dds();
    auto tethys_buff = get_res_tethys_dds();
    auto iapetus_buff = get_res_iapetus_dds();
    auto mimas_buff = get_res_mimas_dds();
    auto uranus_buff = get_res_uranus_dds();
    auto neptune_buff = get_res_neptune_dds();



	Planet sun;
	sun.setup("sun",
			  sun_size,
			  0.f,
			  0.f,
			  25.6f * day,
	{ 0.f, 0.f, 7.25f * degree },
			  SUN,
              sun_buff->buffer, sun_buff->size);

	Planet mercury;
	mercury.setup("mercury",
				  mercury_size,
				  mercury_distance,
				  day * 88.f,
				  day * 58.6f,
	{0.f, 0.f, 0.1 * degree},
				  PLANET,
                  mercury_buff->buffer, mercury_buff->size);

	Planet venus;
	venus.setup("venus",
				venus_size,
				venus_distance,
				day * 224.701f,
				day * 116.75f,
	{0.f, 0.f, 177.f * degree},
				PLANET,
                venus_buff->buffer, venus_buff->size);

	Planet earth;
	earth.setup("earth",
				earth_size,
				earth_distance,
				year,
				day,
	{0.f, 0.f, 23.f * degree},
				PLANET,
                earth_buff->buffer, earth_buff->size);



	Planet moon;
	moon.setup("moon",
			   moon_size,
			   moon_distance,
			   day * 29.530589f,
			   0.f,
	{0.f, 0.f, 6.687f * degree},
			   MOON,
               moon_buff->buffer, moon_buff->size);

	earth.setChildrenCount(1);
	earth.setChild(0, &moon);


	Planet mars;
	mars.setup("mars",
			   mars_size,
			   mars_distance,
			   686.971f * day,
			   1.02876421707f * day,
	{0.f, 0.f, 25.f * degree},
			   PLANET,
               mars_buff->buffer, mars_buff->size);



	Planet jupiter;
	jupiter.setup("jupiter",
				  jupiter_size,
				  jupiter_distance,
				  11.86f * year,
				  9.97f * hour,
	{0.f, 0.f, 3.f * degree},
				  PLANET,
                  jupiter_buff->buffer, jupiter_buff->size);


	Planet europa;
	europa.setup("europa",
				 europa_size,
				 europa_distance,
				 3.551181f * day,
				 0.f,
	{0.f, 0.f, 0.1f * degree},
				 MOON,
                 europa_buff->buffer, europa_buff->size);


	Planet io;
	io.setup("io",
			 io_size,
			 io_distance,
			 1.769137786f * day,
			 0.f,
	{0.f, 0.f, 0.05f * degree},
			 MOON,
             io_buff->buffer, io_buff->size);


	Planet ganymede;
	ganymede.setup("ganymede",
				   ganymede_size,
				   ganymede_distance,
				   7.15455296f * day,
				   0.f,
	{0.f, 0.f, 2.214f * degree},
				   MOON,
                   ganymede_buff->buffer, ganymede_buff->size);



	Planet callisto;
	callisto.setup("callisto",
				   callisto_size,
				   callisto_distance,
				   16.6890184f * day,
				   0.f,
	{0.f, 0.f, 2.017f * degree},
				   MOON,
                   callisto_buff->buffer, callisto_buff->size);

	jupiter.setChildrenCount(4);
	jupiter.setChild(0, &europa);
	jupiter.setChild(1, &io);
	jupiter.setChild(2, &ganymede);
	jupiter.setChild(3, &callisto);


	Planet saturn;
	saturn.setup("saturn",
				 saturn_size,
				 saturn_distance,
				 29.f * year,
				 10.65f * hour,
	{0.f, 0.f, 26.7f * degree},
				 PLANET,
                 saturn_buff->buffer, saturn_buff->size);

	Planet titan;
	titan.setup("titan",
				2576.f,
				1221870.f,
				15.945f * day,
				0.f,
	{0.f, 0.f, 0.f * degree},
				MOON,
                titan_buff->buffer, titan_buff->size);

	Planet enceladus;
	enceladus.setup("enceladus",
					252.f,
					238020.f,
					1.370218f * day,
					0.f,
	{0.f, 0.f, 0.017f * degree},
					MOON,
                    enceladus_buff->buffer, enceladus_buff->size);

	Planet rhea;
	rhea.setup("rhea",
			   763.8f,
			   527108.f,
			   4.518212f * day,
			   0.f,
	{0.f, 0.f, 0.f * degree},
			   MOON,
               rhea_buff->buffer, rhea_buff->size);

	Planet dione;
	dione.setup("dione",
				561.4f,
				377400.f,
				2.736915f * day,
				0.f,
	{0.f, 0.f, 0.f * degree},
				MOON,
                dione_buff->buffer, dione_buff->size);

	Planet tethys;
	tethys.setup("tethys",
				 531.1f,
				 294619.f,
				 1.887802f * day,
				 0.f,
	{0.f, 0.f, 0.f * degree},
				 MOON,
                 tethys_buff->buffer, tethys_buff->size);

	Planet iapetus;
	iapetus.setup("iapetus",
				  734.5f,
				  3560820.f,
				  79.3215f * day,
				  0.f,
	{0.f, 0.f, 0.f * degree},
				  MOON,
                  iapetus_buff->buffer, iapetus_buff->size);

	Planet mimas;
	mimas.setup("mimas",
				198.2f,
				185539.f,
				0.942f * day,
				0.f,
	{0.f, 0.f, 0.f * degree},
				MOON,
                mimas_buff->buffer, mimas_buff->size);

	saturn.setChildrenCount(7);
	saturn.setChild(0, &titan);
	saturn.setChild(1, &enceladus);
	saturn.setChild(2, &rhea);
	saturn.setChild(3, &dione);
	saturn.setChild(4, &tethys);
	saturn.setChild(5, &iapetus);
	saturn.setChild(6, &mimas);


	Planet uranus;
	uranus.setup("uranus",
				 25362.f,
				 2870671400.f,
				 84.016846f * year,
				 17.233333333f * hour,
	{0.f, 0.f, 97.77f * degree},
				 PLANET,
                 uranus_buff->buffer, uranus_buff->size);

	Planet neptune;
	neptune.setup("neptune",
				  24622.f,
				  4503000000.f,
				  165 * year,
				  16.266666667f * hour,
	{0.f, 0.f, 28.32f * degree},
				  PLANET,
                  neptune_buff->buffer, neptune_buff->size);


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


	//	m_fire->setProperties(
	//				glm::vec3(0.f, 0.f, 0.f), // Where the particles are generated
	//				glm::vec3(0.f, -0.2, 0.f), // Minimal velocity
	//				glm::vec3(0.f, -0.4, 0.f), // Maximal velocity
	//				glm::vec3(0.f, -1.1, 0.f), // Gravity force applied to particles
	//				glm::vec3(0.f, 1.f, 0.f), // Color (light blue)
	//				2000.f, // Minimum lifetime in seconds
	//				5000.0f, // Maximum lifetime in seconds
	//				0.5f, // Rendered size
	//				jupiter.calc_size / 2,
	//				1.f, // Spawn every 0.05 seconds
	//				20); // And spawn 30 particles




	//	float mercury_scaler = mercury_distance * 100/ system_size;
	//	float mercury_ratio = sun_size / mercury_size;

	//	float venus_scaler = venus_distance * 100 / system_size;
	//	float venus_ratio = sun_size / venus_size;


	//	float earth_scaler = earth_distance * 100 / system_size;
	//	float earth_ratio = sun_size / earth_size;

	//	float moon_scaler =  moon_distance * 100 / system_size;// std::pow(std::log10(moon_distance), 2.f) / std::pow(std::log2(moon_distance), 2.f) ;
	//	float moon_ratio = earth_size /  moon_size;

	//	float mars_scaler = mars_distance * 100 / system_size;
	//	float mars_ratio = sun_size / mars_size;

	//	float jupiter_scaler = jupiter_distance * 100 / system_size;
	//	float jupiter_ratio = sun_size / jupiter_size;

	//	float europa_scaler = europa_distance * 100.f / system_size;// std::pow(std::log10(europa_distance), 2.f) / std::pow(std::log2(europa_distance), 2.f) ;
	//	float europa_ratio = jupiter_size / europa_size;


	//	float mercury_distance_calc = mercury_scaler * system_scale;
	//	float venus_distance_calc = venus_scaler * system_scale; //scaling(venus_ratio, venus_scaler);
	//	float earth_distance_calc = earth_scaler * system_scale; //scaling(earth_ratio, earth_scaler);
	//	float moon_distance_calc = moon_scaler * earth_ratio; //scaling(moon_ratio, moon_scaler);
	//	float mars_distance_calc = mars_scaler * system_scale; //scaling(mars_ratio, mars_scaler);
	//	float jupiter_distance_calc = jupiter_scaler * system_scale;
	//	float europa_distance_calc = europa_scaler * jupiter_ratio;

	//	float log_size_sun =  system_scale / sun_size * 35000.f;
	//	float log_size_mercury = 800.f / mercury_ratio ; //std::log10(mercury_ratio) * std::log2(mercury_ratio) * mercury_scaler * 15.f;
	//	float log_size_venus = 800.f / venus_ratio;//std::log10(venus_ratio) * std::log2(venus_ratio) * venus_scaler * 15.f;
	//	float log_size_earth = 800.f / earth_ratio;//std::log10(earth_ratio) * std::log2(earth_ratio) * earth_scaler * 15.f;
	//	float log_moon_size =  8.f /  moon_ratio;//std::log10(moon_ratio) * std::log2(moon_ratio) * moon_scaler * 75.f;
	//	float log_size_mars = 800.f / mars_ratio; //std::log10(mars_ratio) * std::log2(mars_ratio) * mars_scaler * 15.f;
	//	float log_size_jupiter = 800.f / jupiter_ratio;
	//	float log_size_europa = 8.f / europa_ratio;


	//	float sun_size_scale =  log_size_sun; //scaling(sun_size / 10000.f, 100.f);
	//	float mercury_size_scale = log_size_mercury; //scaling(15, mercury_size * 100.f / sun_size);
	//	float venus_size_scale = log_size_venus; //scaling(35, venus_size * 100.f / sun_size);
	//	float earth_size_scale = log_size_earth;//scaling(earth_ratio, earth_size * 100.f / sun_size);
	//	float moon_size_scale = log_moon_size;//scaling(moon_ratio, moon_size * 100.f / earth_size);


	//	Mesh sun(sphare::create(sun_size_scale, 256, 256));
	//	sun.addTexture(Texture(GL_TEXTURE_2D, "textures/sun.jpg"));

	//	Model sun_model( { sun });
	////	sun_model.translate({50.f, 50.f, 50.f});

	//	Mesh mercury(sphare::create(mercury_size_scale, 256, 256));
	//	mercury.addTexture(Texture(GL_TEXTURE_2D, "textures/mercurymap.jpg"));

	//	Model mercury_model ({ mercury });
	//	mercury_model.setLocation({sun_size_scale + mercury_size_scale + mercury_distance_calc, 0.f, 0.f});

	//	planet_data *mercury_data = new planet_data;
	//	mercury_data->orbiral_speed = day * 88.f;
	//	mercury_data->axis_rot_speed = day * 58.6f;
	//	mercury_data->axis_tilt.z = 0.1f * degree;

	//	mercury_model.setModelPreProcFunction(__preproc_motion);
	//	mercury_model.setModelPostProcFunction(__postproc_motion);
	//	mercury_model.setModelData(mercury_data);


	//	Mesh venus(sphare::create(venus_size_scale, 256, 256));
	//	venus.addTexture(Texture(GL_TEXTURE_2D, "textures/venusmap.jpg"));

	//	Model venus_model ({ venus });
	//	venus_model.setLocation({sun_size_scale + venus_size_scale + venus_distance_calc, 0.f, 0.f});

	//	planet_data *venus_data = new planet_data;
	//	venus_data->orbiral_speed = day * 224.701f ;
	//	venus_data->axis_rot_speed = day * 116.75f;
	//	venus_data->axis_tilt.z = 177.f * degree;

	//	venus_model.setModelPreProcFunction(__preproc_motion);
	//	venus_model.setModelPostProcFunction(__postproc_motion);
	//	venus_model.setModelData(venus_data);


	//	Mesh earth(sphare::create(earth_size_scale, 256, 256));
	//	earth.addTexture(Texture(GL_TEXTURE_2D, "textures/earth.dds"));

	//	Model earth_model ({ earth });
	//	earth_model.setLocation({sun_size_scale + earth_size_scale + earth_distance_calc, 0.f, 0.f});


	//	planet_data *earth_data = new planet_data;
	//	earth_data->orbiral_speed = year ;
	//	earth_data->axis_rot_speed = day;
	//	earth_data->axis_tilt.z = 23.f * degree;

	//	earth_model.setModelPreProcFunction(__preproc_motion);
	//	earth_model.setModelPostProcFunction(__postproc_motion);
	//	earth_model.setModelData(earth_data);

	//	Mesh moon(sphare::create( moon_size_scale, 128, 128));
	//	moon.addTexture(Texture(GL_TEXTURE_2D, "textures/moon.dds"));

	//	Model moon_model ({ moon });
	//	moon_model.setLocation({earth_size_scale + moon_size_scale +  moon_distance_calc, 0.f, 0.f});

	//	planet_data *moon_data = new planet_data;
	//	moon_data->orbiral_speed = day * 29.530589f;
	//	moon_data->axis_rot_speed = day * 27.321582f;

	//	moon_model.setModelPreProcFunction(__preproc_motion);
	//	moon_model.setModelPostProcFunction(__postproc_motion);
	//	moon_model.setModelData(moon_data);

	//	Mesh mars(sphare::create(log_size_mars, 256, 256));
	//	mars.addTexture(Texture(GL_TEXTURE_2D, "textures/mars.dds"));

	//	Model mars_model ({ mars });
	//	mars_model.setLocation({sun_size_scale + log_size_mars + mars_distance_calc, 0.f, 0.f});


	//	planet_data *mars_data = new planet_data;
	//	mars_data->orbiral_speed = 686.971f * day;
	//	mars_data->axis_rot_speed = 1.02876421707f * day;
	//	mars_data->axis_tilt.z = 25.f * degree;

	//	mars_model.setModelPreProcFunction(__preproc_motion);
	//	mars_model.setModelPostProcFunction(__postproc_motion);
	//	mars_model.setModelData(mars_data);

	//	Mesh jupiter(sphare::create(log_size_jupiter, 256, 256));
	//	jupiter.addTexture(Texture(GL_TEXTURE_2D, "textures/jupiter.jpg"));

	//	Model jupiter_model ({ jupiter });
	//	jupiter_model.setLocation({sun_size_scale + log_size_jupiter + jupiter_distance_calc, 0.f, 0.f});


	//	planet_data *jupiter_data = new planet_data;
	//	jupiter_data->orbiral_speed =  11.86f * year;
	//	jupiter_data->axis_rot_speed = 9.97f * hour;
	//	jupiter_data->axis_tilt.z = 3.f * degree;

	//	jupiter_model.setModelPreProcFunction(__preproc_motion);
	//	jupiter_model.setModelPostProcFunction(__postproc_motion);
	//	jupiter_model.setModelData(jupiter_data);


	//	Mesh europa(sphare::create(log_size_europa, 256, 256));
	//	europa.addTexture(Texture(GL_TEXTURE_2D, "textures/io.dds"));

	//	Model europa_model ({ europa });
	//	europa_model.setLocation({log_size_jupiter + log_size_europa + europa_distance_calc, 0.f, 0.f});

	//	planet_data *europa_data = new planet_data;
	//	europa_data->orbiral_speed =  3.551181f * day;
	//	europa_data->axis_rot_speed = 3.551181f * day;
	//	europa_data->axis_tilt.z = 0.1f * degree;


	//	europa_model.setModelPreProcFunction(__preproc_motion);
	//	europa_model.setModelPostProcFunction(__postproc_motion);
	//	europa_model.setModelData(europa_data);

	//	planet_data *lo_data = new planet_data;
	//	lo_data->orbiral_speed =  1.769137786f * day;
	//	lo_data->axis_rot_speed = 1.769137786f * day;
	//	lo_data->axis_tilt.z = 0.05f * degree;


	//	planet_data *callisto_data = new planet_data;
	//	callisto_data->orbiral_speed =  16.6890184f * day;
	//	callisto_data->axis_rot_speed = 16.6890184f * day;
	//	callisto_data->axis_tilt.z = 2.017f * degree;

	//	planet_data *ganymede_data = new planet_data;
	//	ganymede_data->orbiral_speed =  7.15455296f * day;
	//	ganymede_data->axis_rot_speed = 7.15455296f * day;
	//	ganymede_data->axis_tilt.z = 2.214f * degree;




	//	sun_model.addChild(std::move(mercury_model));

	//	sun_model.addChild(std::move(venus_model));

	//	earth_model.addChild(std::move(moon_model));
	//	sun_model.addChild(std::move(earth_model));

	//	sun_model.addChild(std::move(mars_model));

	//	jupiter_model.addChild(std::move(europa_model));
	//	sun_model.addChild(std::move(jupiter_model));


	//	m_models.push_back(std::move(sun_model));

	rings.init();

    auto font_data = get_res_freesans_ttf();
    m_text = { font_data->buffer, font_data->size, 26 };
    glDebugger::init({ font_data->buffer, font_data->size, 14 });
    delete font_data;

	m_mouse = { m_window };
	m_keyboard = { m_window };



	m_mouse.onClickLeft(std::bind(&glProgram::handleSelection, this, std::placeholders::_1, std::placeholders::_2));

	m_mouse.onDragLeft([=](MoveDirection event, float diff)
	{
		if(event == MOVE_Y)
		{
			m_camera.move(diff);
		}
		else if(event == MOVE_X)
		{
			m_camera.strafe(diff);
		}
		else if(event == MOVE_Z)
		{
			m_camera.zoom(diff);
		}
	});

	m_mouse.onDragRight([=](MoveDirection event, float diff)
	{
		if(event == MOVE_Y)
		{
			m_camera.rotateVertical(diff);
		}
		else if(event == MOVE_X)
		{
			m_camera.rotateHorizontal(diff);
		}
		else if(event == MOVE_Z)
		{
			m_camera.zoom(diff);
		}
	});

	m_keyboard.moveX([=](float diff)
	{

		m_camera.move(diff);
	});

	m_keyboard.moveY([=](float diff)
	{
		m_camera.strafe(diff);
	});

	m_keyboard.moveZ([=](float diff)
	{
		m_camera.zoom(diff);
	});

	m_keyboard.rotateHorizontal([=](float diff)
	{
		m_camera.rotateHorizontal(diff);
	});

	m_keyboard.rotateVertical([=](float diff)
	{
		m_camera.rotateVertical(diff);
	});

	glfwSwapInterval(1);

}


void glProgram::exec()
{
	int frameCount = 0;
	auto startTime = std::chrono::high_resolution_clock::now();
	do
	{

		if(glfwGetKey(m_window, GLFW_KEY_1) == GLFW_PRESS)
		{
			for(auto &m : m_models)
			{
				m.setAnimation(0);
			}
		}
		else if(glfwGetKey(m_window, GLFW_KEY_2) == GLFW_PRESS)
		{
			for(auto &m : m_models)
			{
				m.setAnimation(1);
			}
		}

		if(glfwGetKey(m_window, GLFW_KEY_0) == GLFW_PRESS)
		{
			for(auto &m : m_models)
			{
				m.setAnimation(-1);
			}
		}

		m_mouse.update();
		m_keyboard.update();
		render();
		glfwPollEvents();
		++frameCount;
		m_frameRate = frameCount /
					  ( std::chrono::duration_cast<std::chrono::duration<float> >(
							std::chrono::high_resolution_clock::now() -
							startTime).count());
		if(frameCount == 600)
		{
			startTime = std::chrono::high_resolution_clock::now();
			frameCount = 0;
		}

	}
	while (glfwGetKey(m_window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(m_window) == 0 );
}


void glProgram::render()
{
	Clock::update();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_camera.calcViewport();

	m_axis.render(m_camera);

	//    m_axisProgram.use();
	//    m_camera.update(m_axisProgram);
	//    m_axis.render(m_axisProgram);

	//	m_skyProgram.use();
	//	m_camera.update(m_skyProgram);
	//	m_box.render(m_camera);


	m_objectProgram.use();
	m_lamp.update(m_objectProgram);
	m_camera.update(m_objectProgram);



	for(auto &m : m_models)
	{
		m.render(m_objectProgram);
	}

	rings.render(m_camera);

	//	m_fire->update();
	//	m_fire->render(m_camera);

	char buf[128];
	std::sprintf(buf,"FPS: %.2f", m_frameRate);
	m_text.render(buf, glm::vec4(0.f, 0.f, 0.f, 1.f), 20, 40);
	//	std::sprintf(buf,"PC: %d", m_fire->count());
	//	m_text.render(buf, glm::vec4(0.f, 0.f, 0.f, 1.f), 20, 80);

	//	glDebugger::flush();


	//    m_smoke->update();
	//    m_smoke->render(m_camera);

	glfwSwapBuffers(m_window);
}

void glProgram::handleSelection(double x, double y)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_objectSelectionProgram.use();
	m_camera.update(m_objectSelectionProgram);

	for(auto &m : m_models)
	{
		m.render(m_objectSelectionProgram);
	}

	unsigned char res[4] { 0 };

	double viewportheight = m_objectSelectionProgram.getViewport().h;
	double screenX = x;
	double screenY =  viewportheight - y;
	glReadPixels(screenX, screenY, 1,1,GL_RGBA, GL_UNSIGNED_BYTE, res);

	auto it = std::find(std::begin(m_models), std::end(m_models), ColourID(res[0], res[1], res[2]).value());
	if(it != std::end(m_models))
	{
		it->select(!it->selected());
	}
}
