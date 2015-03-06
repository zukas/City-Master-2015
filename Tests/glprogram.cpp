#include "glprogram.h"
#include <GL/glew.h>
#include <algorithm>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "loader.h"
#include "clock.h"


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

    glewExperimental = true;

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

    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    m_objectProgram = { "shaders/object.vert", "shaders/object.frag" };
    m_objectSelectionProgram = {"shaders/object_select.vert", "shaders/object_select.frag", Selection };
    m_axisProgram = { "shaders/basic.vert", "shaders/basic.frag" };

    Model _m1 = load("models/cube.dae");
    m_models.push_back(std::move(_m1));

    _m1 = load("models/cube.dae");
    _m1.translate({2.f, 0.5f, 2.f});
    m_models.push_back(std::move(_m1));

    m_axis = Axis(1000.f);


    for(auto &m : m_models)
    {
        m.rotate(PI / 2, { -1.f, 0.f, 0.f });
    }

}


void glProgram::exec()
{
    do
    {
        static bool track = true;
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
        if(track && glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            track = false;
            mouseTrack();
        }
        else if(!track  && glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
        {
            track = true;
        }

        render();
        glfwPollEvents();

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

    m_objectProgram.use();
    m_lamp.update(m_objectProgram);
    m_camera.update(m_objectProgram);

    for(auto &m : m_models)
    {
        m.render(m_objectProgram);
    }

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
    GLint viewport[4];

    renderSelection();

    glGetIntegerv(GL_VIEWPORT, viewport);
    glReadPixels(xPos, viewport[3] - yPos, 1,1,GL_RGBA, GL_UNSIGNED_BYTE, &res);

    auto it = std::find(std::begin(m_models), std::end(m_models), ColourID(res[0], res[1], res[2]).value());
    if(it != std::end(m_models))
    {
        it->select(!it->selected());
    }
}
