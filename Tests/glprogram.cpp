#include "glprogram.h"
#include <GL/glew.h>
#include <algorithm>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "loader.h"
#include "clock.h"

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

    glClearColor(0.9f, 0.9f, 0.9f, 0.1f);

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
    printf("GL_ARB_texture_cube_map: %d\n", glewGetExtension("GL_ARB_texture_cube_map") == GL_TRUE);

    printf("GL_ARB_seamless_cube_map: %d\n", glewGetExtension("GL_ARB_seamless_cube_map") == GL_TRUE);





    m_objectProgram = { "shaders/object.vert", "shaders/object.frag" };
    m_objectSelectionProgram = {"shaders/object_select.vert", "shaders/object_select.frag", Selection };
    m_axisProgram = { "shaders/basic.vert", "shaders/basic.frag" };
    m_textProgram = { "shaders/text.vert", "shaders/text.frag" };
    m_skyProgram = { "shaders/skybox.vert", "shaders/skybox.frag" };

    m_box =
    {
        "textures/left.jpg",
        "textures/right.jpg",
        "textures/down.jpg",
        "textures/up.jpg",
        "textures/front.jpg",
        "textures/back.jpg"
    };

    Model mod = load("models/cube.dae");

    for(float i = -30.f; i <= 30.f; i += 10.f)
    {
        for(float j = -30.f; j <= 30.f; j += 10.f)
        {
            Model m = mod;
            m.translate({i, 0.f, j});
            m_models.push_back(std::move(m));
        }
    }

    for(auto &m : m_models)
    {
        m.rotate(PI / 2, { -1.f, 0.f, 0.f });
    }

    m_axis = Axis(1000.f);
    m_text = { "fonts/FreeSans.ttf", 26 };

    m_mouse = { m_window };

    m_mouse.onclick([=](double x, double y)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_objectSelectionProgram.use();
        m_camera.update(m_objectSelectionProgram);

        for(auto &m : m_models)
        {
            m.render(m_objectSelectionProgram);
        }

        unsigned char res[4] { 0 };
        glReadPixels(x, m_objectSelectionProgram.getViewport().h - y, 1,1,GL_RGBA, GL_UNSIGNED_BYTE, &res);

        auto it = std::find(std::begin(m_models), std::end(m_models), ColourID(res[0], res[1], res[2]).value());
        if(it != std::end(m_models))
        {
            it->select(!it->selected());
        }
    });

    glfwSwapInterval(0);

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
        render();
        glfwPollEvents();
        ++frameCount;
        m_frameRate = float(frameCount) /
                      ( std::chrono::duration_cast<std::chrono::duration<float> >(
                            std::chrono::high_resolution_clock::now() -
                            startTime).count());

    }
    while (glfwGetKey(m_window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(m_window) == 0 );
}


void glProgram::render()
{
    Clock::update();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_camera.calcViewport();

    m_axisProgram.use();
    m_camera.update(m_axisProgram);
    m_axis.render(m_axisProgram);

    m_skyProgram.use();
    m_camera.update(m_skyProgram);
    m_skyProgram.setCameraPosition(m_camera.position());
    m_box.render(m_skyProgram);


    m_objectProgram.use();
    m_lamp.update(m_objectProgram);
    m_camera.update(m_objectProgram);

    for(auto &m : m_models)
    {
        m.render(m_objectProgram);
    }

    glDisable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);

    m_textProgram.use();
    //    m_textProgram.setUniform(glsl_projection_matrix, glm::ortho(0.0f, float(windowWidth), 0.0f, float(windowHeight)));
    m_textProgram.setProjectionMatrix(glm::ortho(0.0f, float(windowWidth), 0.0f, float(windowHeight)));

    char buf[128];
    int length = std::sprintf(buf,"FPS: %.2f", m_frameRate);
    m_text.render(m_textProgram,glm::vec4(0.5f, 0.2f, 0.6f, 1.f), std::string(buf, length), 20, 40);


    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glfwSwapBuffers(m_window);
}

void glProgram::renderSelection()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_objectSelectionProgram.use();
    m_camera.update(m_objectSelectionProgram);

    for(auto &m : m_models)
    {
        m.render(m_objectSelectionProgram);
    }
}

void glProgram::mouseTrack()
{
    double xPos = 0.;
    double yPos = 0.;
    glfwGetCursorPos(m_window, &xPos, &yPos);

    unsigned char res[4];


    renderSelection();
    glReadPixels(xPos, m_objectSelectionProgram.getViewport().h - yPos, 1,1,GL_RGBA, GL_UNSIGNED_BYTE, &res);

    auto it = std::find(std::begin(m_models), std::end(m_models), ColourID(res[0], res[1], res[2]).value());
    if(it != std::end(m_models))
    {
        it->select(!it->selected());
    }
}
