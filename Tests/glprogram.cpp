#include "glprogram.h"
#include <GL/glew.h>
#include <algorithm>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "loader.h"
#include "clock.h"
#include "gldebugger.h"

#include <chrono>


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
    m_lamp = { glm::vec3(46.6, 77.2, 54.7), 82.4 };

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

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

    m_objectProgram = { "shaders/object.vert", "shaders/object.frag" };
    m_objectSelectionProgram = {"shaders/object_select.vert", "shaders/object_select.frag", Selection };
    m_axisProgram = { "shaders/basic.vert", "shaders/basic.frag" };
    m_textProgram = { "shaders/text.vert", "shaders/text.frag" };
    m_skyProgram = { "shaders/skybox.vert", "shaders/skybox.frag" };

    m_fire = new ParticleSystem;
    Texture pt { GL_TEXTURE_2D, "textures/fire2.jpg"};
//    pt.setSamplerParameter(GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
    m_fire->setTexture(pt);
    m_fire->setBlendFunc(GL_SRC_ALPHA, GL_ONE);
    m_fire->setProperties(
                glm::vec3(-.75f, 5.f, -.75f), // Where the particles are generated
                glm::vec3(0.f, -0.2, 0.f), // Minimal velocity
                glm::vec3(0.f, -0.4, 0.f), // Maximal velocity
                glm::vec3(0.f, -1.1, 0.f), // Gravity force applied to particles
                glm::vec3(0.f, 0.28235f, 1.f), // Color (light blue)
                4000.f, // Minimum lifetime in seconds
                8000.0f, // Maximum lifetime in seconds
                0.02f, // Rendered size
                1.f, // Spawn every 0.05 seconds
                5); // And spawn 30 particles

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

    Model mod = load("models/sphere.dae");
        m_models.push_back(std::move(mod));
//    for(float i = -150.f; i <= 150.f; i += 150.f)
//    {
//        for(float j = -150.f; j <= 150.f; j += 150.f)
//        {
//            Model m = mod;
//            m.translate({i, 0.f, j});
//            m_models.push_back(std::move(m));
//        }
//    }

    for(auto &m : m_models)
    {
        m.rotate(PI / 2, { -1.f, 0.f, 0.f });
    }

    m_axis = Axis(1000.f);
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

//    m_axisProgram.use();
//    m_camera.update(m_axisProgram);
//    m_axis.render(m_axisProgram);

	m_skyProgram.use();
	m_camera.update(m_skyProgram);
	m_box.render(m_skyProgram);


	m_objectProgram.use();
	m_lamp.update(m_objectProgram);
	m_camera.update(m_objectProgram);

	for(auto &m : m_models)
	{
		m.render(m_objectProgram);
	}

    m_fire->update();
    m_fire->render(m_camera);

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
