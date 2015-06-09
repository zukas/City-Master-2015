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

#include <chrono>

constexpr float year { 360000.f };
constexpr float size_div { 6371.f };
constexpr float distance_div { 14959.78707f };
constexpr float rat { distance_div / size_div };
constexpr float days_in_year { 365.256f };
constexpr float day { year / days_in_year };

//glm::mat4 rot(const glm::mat4 &parent, )


glm::mat4 __planet_motion(const glm::mat4 &model, const glm::mat4 &parent, float current, float duration)
{
	glm::mat4 mat = glm::mat4_cast(glm::angleAxis(PI2 * (current / duration ), glm::vec3(0.f, 1.f, 0.f)));


	glm::mat4 tr = glm::translate(glm::mat4(1.f), glm::vec3(parent[3])) * mat;
	glm::mat4 mod = (tr * model * glm::translate(glm::mat4(1.f), -glm::vec3(parent[3])));

	return  mod;
}

glm::mat4 __planet_rotation(const glm::mat4 &model, const glm::mat4 &, float current, float duration)
{
	return model * glm::mat4_cast(glm::angleAxis(PI2 * current  / duration, glm::vec3(0.f, 1.f, 0.f)));
}

glm::mat4 __moon_motion(const glm::mat4 &model, const glm::mat4 &parent, float current, float duration)
{
	glm::vec3 parent_rot_vec;
	float parent_rot;
	glm::axisAngle(parent, parent_rot_vec, parent_rot);

	glm::mat4 model_trans = glm::rotate(model, -parent_rot, parent_rot_vec);


	glm::mat4 mat = glm::mat4_cast(glm::angleAxis(PI2 * (current / duration ), glm::vec3(0.f, 1.f, 0.f)));
//	glm::mat4 mat2 = glm::mat4_cast(glm::angleAxis(PI2 * current  / (duration / days_in_year), glm::vec3(0.f, 1.f, 0.f)));

	glm::mat4 tr = glm::translate(glm::mat4(1.f), glm::vec3(parent[3])) * mat;
	glm::mat4 mod = (tr * model_trans * glm::translate(glm::mat4(1.f), -glm::vec3(parent[3]))) * mat;

	mod = glm::rotate(mod, parent_rot, parent_rot_vec);

	return mod;
}

float galactic_scale(float parent, float size, float distance)
{
	return (std::log2(size) * ((std::cos(size) + 1) / 2) - std::log10(parent)) * std::log10(distance / distance_div);
}

float scaling(float amp, float distance)
{
	float pw = std::pow(distance, 2.f);
	return (pw / std::exp(pw * 0.001f)) * amp * 750.f;
//	return (std::cos(std::sqrt(pw)) / std::exp(pw*0.1f)) * amp + amp * distance;
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

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);

	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);

	glEnable(GL_MULTISAMPLE);
//	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	m_objectProgram = { "shaders/object.vert", "shaders/object.frag" };
	m_objectSelectionProgram = {"shaders/object_select.vert", "shaders/object_select.frag", Selection };
	m_axisProgram = { "shaders/basic.vert", "shaders/basic.frag" };
	m_textProgram = { "shaders/text.vert", "shaders/text.frag" };
	//	m_skyProgram = { "shaders/skybox.vert", "shaders/skybox.frag" };

	m_axis.init();
	m_fire = new ParticleSystem;
	Texture pt { GL_TEXTURE_2D, "textures/fire2.jpg"};
	//    pt.setSamplerParameter(GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
	m_fire->setTexture(pt);
	m_fire->setBlendFunc(GL_SRC_ALPHA, GL_ONE);
	m_fire->setProperties(
				glm::vec3(-2.5f, 5.f, -2.5f), // Where the particles are generated
				glm::vec3(0.f, -0.2, 0.f), // Minimal velocity
				glm::vec3(0.f, -0.4, 0.f), // Maximal velocity
				glm::vec3(0.f, -1.1, 0.f), // Gravity force applied to particles
				glm::vec3(1.f, 0.549f, 0.15686f), // Color (light blue)
				12000.f, // Minimum lifetime in seconds
				15000.0f, // Maximum lifetime in seconds
				0.08f, // Rendered size
				1.f, // Spawn every 0.05 seconds
				10); // And spawn 30 particles

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

	m_box =
	{
		"textures/bluecloud_lf.jpg",
		"textures/bluecloud_rt.jpg",
		"textures/bluecloud_dn.jpg",
		"textures/bluecloud_up.jpg",
		"textures/bluecloud_ft.jpg",
		"textures/bluecloud_bk.jpg"
	};

//	for(float i = 0.f; i < 100; i+=0.01f)
//	{
//		std::printf("ripple x: %f, z: %f\n", i, ripple_z(i, 0.f));
//	}

//	fflush(stdout);

	float system =  143.73 * std::pow(10, 9);
	float mercury_distance = 57910000.f;
	float venus_distance = 108200000.f;
	float earth_distance = 149597870.691f;
	float moon_distance = 384400.f;

	float sun_size = 696000.f;
	float mercury_size = 2440.f;
	float venus_size = 6052.f;
	float earth_size = 6371.f;
	float moon_size = 1737.1f;


	float mercury_scaler = mercury_distance * 100/ system;
	float mercury_ratio = sun_size / mercury_size;

	float venus_scaler = venus_distance * 100 / system;
	float venus_ratio = sun_size / venus_size;


	float earth_scaler = earth_distance * 100 / system;
	float earth_ratio = sun_size / earth_size;

	float moon_scaler =  std::pow(std::log10(moon_distance), 2.f) / std::pow(std::log2(moon_distance), 2.f) ;
	float moon_ratio = earth_size /  moon_size;


	float mercury_distance_calc = scaling(mercury_ratio, mercury_scaler);
	float venus_distance_calc = scaling(venus_ratio, venus_scaler);
	float earth_distance_calc = scaling(earth_ratio, earth_scaler);
	float moon_distance_calc = scaling(moon_ratio, moon_scaler);

	float log_size_sun = std::log10(sun_size) * std::log2(sun_size);
	float log_size_mercury = std::log10(mercury_ratio) * std::log2(mercury_ratio) * mercury_scaler * 15.f;
	float log_size_venus = std::log10(venus_ratio) * std::log2(venus_ratio) * venus_scaler * 15.f;
	float log_size_earth = std::log10(earth_ratio) * std::log2(earth_ratio) * earth_scaler * 15.f;
	float log_moon_size =  std::log10(moon_ratio) * std::log2(moon_ratio) * moon_scaler * 75.f;



	float sun_size_scale =  log_size_sun; //scaling(sun_size / 10000.f, 100.f);
	float mercury_size_scale = log_size_mercury; //scaling(15, mercury_size * 100.f / sun_size);
	float venus_size_scale = log_size_venus; //scaling(35, venus_size * 100.f / sun_size);
	float earth_size_scale = log_size_earth;//scaling(earth_ratio, earth_size * 100.f / sun_size);
	float moon_size_scale = log_moon_size;//scaling(moon_ratio, moon_size * 100.f / earth_size);


	Mesh sun(sphare::create(sun_size_scale, 256, 256));
	sun.addTexture(Texture(GL_TEXTURE_2D, "textures/sun.jpg"));

	Model sun_model( { sun });
//	sun_model.translate({50.f, 50.f, 50.f});

	Mesh mercury(sphare::create(mercury_size_scale, 256, 256));
	mercury.addTexture(Texture(GL_TEXTURE_2D, "textures/mercurymap.jpg"));

	Model mercury_model ({ mercury });
	mercury_model.translate({sun_size_scale + mercury_size_scale + mercury_distance_calc, 0.f, 0.f});

	Animation anim0 { __planet_motion, day * 88.f};
	anim0.setSequence(AnimationSequence::REPEAT);
	Animation *mercury_rot = new Animation( __planet_rotation, day * 58.6f );
	mercury_rot->setSequence(AnimationSequence::REPEAT);
	anim0.setNext(mercury_rot);
	mercury_model.addAnimation(std::move(anim0));

	Mesh venus(sphare::create(venus_size_scale, 256, 256));
	venus.addTexture(Texture(GL_TEXTURE_2D, "textures/venusmap.jpg"));

	Model venus_model ({ venus });
	venus_model.translate({sun_size_scale + venus_size_scale + venus_distance_calc, 0.f, 0.f});

	Animation anim01 { __planet_motion, day * 224.701f };
	anim01.setSequence(AnimationSequence::REPEAT);
	Animation *venus_rot = new Animation( __planet_rotation, day * 116.75f);
	venus_rot->setSequence(AnimationSequence::REPEAT);
	anim01.setNext(venus_rot);

	venus_model.addAnimation(std::move(anim01));


	Mesh earth(sphare::create(earth_size_scale, 256, 256));
	earth.addTexture(Texture(GL_TEXTURE_2D, "textures/earth.dds"));

	Model earth_model ({ earth });
	earth_model.translate({sun_size_scale + earth_size_scale + earth_distance_calc, 0.f, 0.f});
//	earth_model.rotate(0.34906585f, glm::vec3(0.f, 0.f, 1.f));

	Animation anim { __planet_motion, year };
	anim.setSequence(AnimationSequence::REPEAT);
	Animation *earth_rot = new Animation( __planet_rotation, day);
	earth_rot->setSequence(AnimationSequence::REPEAT);
	anim.setNext(earth_rot);


	earth_model.addAnimation(std::move(anim));



	Mesh moon(sphare::create( moon_size_scale, 128, 128));
	moon.addTexture(Texture(GL_TEXTURE_2D, "textures/moon.dds"));

	Model moon_model ({ moon });
	moon_model.translate({earth_size_scale + moon_size_scale +  moon_distance_calc, 0.f, 0.f});

	Animation anim2 (__moon_motion, day * 27.322f);
	anim2.setSequence(AnimationSequence::REPEAT);
	moon_model.addAnimation(std::move(anim2));

	earth_model.addChild(std::move(moon_model));


	sun_model.addChild(std::move(mercury_model));

	sun_model.addChild(std::move(venus_model));

	sun_model.addChild(std::move(earth_model));


	m_models.push_back(std::move(sun_model));

	m_text = { "fonts/FreeSans.ttf", 26 };

	glDebugger::init({ "fonts/FreeSans.ttf", 14 });

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

	//	m_fire->update();
	//	m_fire->render(m_camera);

	char buf[128];
	std::sprintf(buf,"FPS: %.2f", m_frameRate);
	m_text.render(buf, glm::vec4(0.f, 0.f, 0.f, 1.f), 20, 40);
	std::sprintf(buf,"PC: %d", m_fire->count());
	m_text.render(buf, glm::vec4(0.f, 0.f, 0.f, 1.f), 20, 80);

	glDebugger::flush();


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
